<%
'******************************************************************************************************************************
'  DESCRIPTION:   Functions for Remedy Case Integration
'  ASSUMPTIONS:   /global/subs/sqlconn.inc
'  NOTES:         Remedy stores customer info as "Customer ID - Customer Name"

'******************************************************************************************************************************
'  DESCRIPTION:
'     Get All Open Remedy Cases for Customer
'     Output: Array/Null
'     Database Columns: 
'     0 - Entry_ID
'     1 - Description
'     2 - Status
'     3 - Submit_Date

Function GetRemedyOpenCases(CustomerID)
   Dim returnValue, resultSet
   returnValue = Null
   
   If Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) Then
      Dim Cmd          '
      
      Call OpenSQLConnection(ctRemedy)
      set Cmd = CreateSQLProcedure(ctRemedy, "[ARSystem].[dbo].[techServices_TechnicalSupport_SelectOpenCases]")
      Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adVarChar, adParamInput, 20, CustomerID & " - %")
      Set resultSet = ExecuteSQLProcedure(ctRemedy, Cmd, True)
      If(resultSet.State = adstateOpen) Then
         If Not resultSet.EOF Then
            returnValue = resultSet.GetRows
         End If
         resultSet.Close
      End If
      Set resultSet = Nothing
      Set Cmd = Nothing
      Call CloseSQLConnection(ctRemedy)
   End If
   GetRemedyOpenCases = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Get Remedy Case Details
'     Input: QueryString("CaseID")
'     Output: Array/Null
'     Database Columns:
'     0 - Entry_ID
'     1 - Description
'     2 - Submit_date  
'     3 - Owner
'     4 - Category
'     5 - Priority
'     6 - Status
'     7 - CDW_Invoice_Number
'     8 - Product_Name
'     9 - ContactName
'     10 - Direct_Contact_Phone_Number

Function GetRemedyCaseDetails(CaseID, CustomerID)
   Dim returnValue, resultSet
   returnValue = Null
   
   If Not IsEmpty(CaseID) and Not IsNull(CaseID) and Len(Trim(CaseID)) = 15 Then
      Dim Cmd      
      
      Call OpenSQLConnection(ctRemedy)
      set Cmd = CreateSQLProcedure(ctRemedy, "[ARSystem].[dbo].[techServices_TechnicalSupport_SelectCase]")
      Cmd.Parameters.Append Cmd.CreateParameter("@CaseNumber", adVarChar, adParamInput, 15, CaseID)
      Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adVarChar, adParamInput, 20, CustomerID & " - %")
      Set resultSet = ExecuteSQLProcedure(ctRemedy, Cmd, True)
      If(resultSet.State = adstateOpen) Then
         If Not resultSet.EOF Then
            returnValue = resultSet.GetRows
         End If
         resultSet.Close
      End If
      Set resultSet = Nothing
      Set Cmd = Nothing
      Call CloseSQLConnection(ctRemedy)
   End If
   GetRemedyCaseDetails = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Get Remedy Case Log
'     Input: QueryString("CaseID")
'     Output: Array/Null
'     Database Columns:
'     0 - Submitter
'     1 - Submit_Date
'     2 - Description
'     3 - Detailed_Description

Function GetRemedyCaseLog(CaseID)
   Dim returnValue, resultSet
   returnValue = Null
   
   If Not(IsEmpty(CaseID)) and Not(IsNull(CaseID)) and Len(Trim(CaseID)) = 15 Then
      Dim Cmd       
     
      Call OpenSQLConnection(ctRemedy)
      set Cmd = CreateSQLProcedure(ctRemedy, "[ARSystem].[dbo].[techServices_TechnicalSupport_SelectCaseLog]")
      Cmd.Parameters.Append Cmd.CreateParameter("@CaseNumber", adVarChar, adParamInput, 20, CaseID)
      Set resultSet = ExecuteSQLProcedure(ctRemedy, Cmd, True)
      If(resultSet.State = adstateOpen) Then
         If Not resultSet.EOF Then
            returnValue = resultSet.GetRows
         End If
         resultSet.Close
      End If
      Set resultSet = Nothing
      Set Cmd = Nothing
      Call CloseSQLConnection(ctRemedy)
   End If
   GetRemedyCaseLog = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Get Remedy Case Details for Customer from Case ID
'     Output: Array/Null
'     Database Columns:
'     0 - Direct_Contact_First_Name
'     1 - Direct_Contact_Last_Name

Function GetRemedyCustomerContacts(CustomerID)
   Dim returnValue, resultSet
   returnValue = Null
   
   If Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) Then
      Dim Cmd      
      
      Call OpenSQLConnection(ctRemedy)
      set Cmd = CreateSQLProcedure(ctRemedy, "[ARSystem].[dbo].[techServices_TechnicalSupport_SelectCustomerContacts]")
      Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adVarChar, adParamInput, 20, CustomerID & " - %")
      Set resultSet = ExecuteSQLProcedure(ctRemedy, Cmd, True)
      If(resultSet.State = adstateOpen) Then
         If Not resultSet.EOF Then
            returnValue = resultSet.GetRows
         End If
         resultSet.Close
      End If
      Set resultSet = Nothing
      Set Cmd = Nothing
      Call CloseSQLConnection(ctRemedy)
   End If
   GetRemedyCustomerContacts = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Search for Remedy Case
'     Output: Array/Null
'     Database Columns:
'     0 - Entry_ID
'     1 - Description
'     2 - Status
'     3 - Priority
'     4 - ContactName
'     5 - Submit_Date

Function GetRemedyCaseSearchResults(CustomerID, CaseID, StartDate, EndDate, Priority, Contact, Keyword)
   Dim returnValue, resultSet
   returnValue = Null
   
   If Not(IsEmpty(CustomerID)) and Not(IsNull(CustomerID)) Then
      Dim Cmd, ContactArr
      
      If Len(Trim(CaseID)) <> 15 Then CaseID = "%"
      If IsEmpty(StartDate) or IsNull(StartDate) or Not IsDate(StartDate) Then StartDate = 1 Else StartDate = ConvertToEpoch(StartDate)
      If IsEmpty(EndDate) or IsNull(EndDate) or Not IsDate(EndDate) Then EndDate = ConvertToEpoch(Now()) Else EndDate = ConvertToEpoch(EndDate)
      If Len(Trim(Contact)) = 0 Then ContactArr = Array("%", "%") Else ContactArr = Split(Replace(Contact, " ", ""), ",", 2)
      If Not IsNumeric(Priority) Then Priority = Null
      If IsEmpty(Keyword) or IsNull(Keyword) Then Keyword = "%" Else Keyword = "%" & Keyword & "%"
         
      Call OpenSQLConnection(ctRemedy)
      set Cmd = CreateSQLProcedure(ctRemedy, "[ARSystem].[dbo].[techServices_TechnicalSupport_SearchCustomerCases]")
      Cmd.Parameters.Append Cmd.CreateParameter("@CustomerNumber", adVarChar, adParamInput, 20, CustomerID & " - %")
      Cmd.Parameters.Append Cmd.CreateParameter("@CaseNumber", adVarChar, adParamInput, 15, CaseID)      
      Cmd.Parameters.Append Cmd.CreateParameter("@StartSubmittedDate", adInteger, adParamInput, , StartDate)
      Cmd.Parameters.Append Cmd.CreateParameter("@EndSubmittedDate", adInteger, adParamInput, , EndDate)
      Cmd.Parameters.Append Cmd.CreateParameter("@Priority", adInteger, adParamInput, , Priority)      
      Cmd.Parameters.Append Cmd.CreateParameter("@ContactFirstName", adVarChar, adParamInput, 30, ContactArr(1)) 
      Cmd.Parameters.Append Cmd.CreateParameter("@ContactLastName", adVarChar, adParamInput, 30, ContactArr(0))     
      Cmd.Parameters.Append Cmd.CreateParameter("@Keyword", adVarChar, adParamInput, 50, Keyword)
   
      Set resultSet = ExecuteSQLProcedure(ctRemedy, Cmd, True)
      If(resultSet.State = adstateOpen) Then
         If Not resultSet.EOF Then
            returnValue = resultSet.GetRows
         End If
         resultSet.Close
      End If
      Set resultSet = Nothing
      Set Cmd = Nothing
      Call CloseSQLConnection(ctRemedy)
   End IF
   GetRemedyCaseSearchResults = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Epoch Time to Standard DateTime
'     Output: Value/Null

Function ConvertFromEpoch(EpochTime)
    Dim returnValue
    returnValue = Null
    
    If Not(IsEmpty(EpochTime)) and Not(IsNull(EpochTime)) and IsNumeric(EpochTime) Then
        returnValue = DateAdd("s", EpochTime, "1/1/1970 00:00:00")
    End If
    ConvertFromEpoch = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Standard DateTime to Epoch Time
'     Output: Value/Null

Function ConvertToEpoch(StandardTime)
    Dim returnValue
    returnValue = Null
    
    If Not(IsEmpty(StandardTime)) and Not(IsNull(StandardTime)) and IsDate(StandardTime) Then
        returnValue = DateDiff("s", "1/1/1970 00:00:00", StandardTime)
    End If
    ConvertToEpoch = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Status Value to Status Name
'     Output: Value/Null

Function FormatRemedyCaseStatus(CaseStatus)
    Dim returnValue
    returnValue = Null
    
    Select Case CaseStatus
    Case "0"
        returnValue = "New" 
    Case "1"
        returnValue = "Assigned"
    Case "2"
        returnValue = "In Progress"
    Case "3"
        returnValue = "Pending"
    Case "4"
        returnValue = "Resolved"
    Case "5"
        returnValue = "Closed"
    Case "6"
        returnValue = "Canceled"
    Case Else
        returnValue = "Unknown"
    End Select

    FormatRemedyCaseStatus = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Priority Value to Priority Name
'     Output: Value/Null

Function FormatRemedyCasePriority(CasePriority)
    Dim returnValue
    returnValue = Null
    
    Select Case CasePriority
    Case "1"
        returnValue = "Critical"
    Case "2"
        returnValue = "High"
    Case "3"
        returnValue = "Medium"
    Case "4"
        returnValue = "Low"
    End Select    

    FormatRemedyCasePriority = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Drop down options for Remedy Priority select box options
'     Output: HTML Text

Function GetRemedyPriorityOptions()
    Dim returnValue
    returnValue = Null

    returnValue   = "<option value='1'>Critical</option>" &_
                    "<option value='2'>High</option>" &_
                    "<option value='3'>Medium</option>" &_
                    "<option value='4'>Low</option>"
    
    GetRemedyPriorityOptions = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Drop down options for Remedy Case Type select box options
'     Output: HTML Text

Function GetRemedyCaseTypeOptions()
    Dim returnValue
    returnValue = Null

    returnValue   = "<option value='1'>Problem</option>" &_
                    "<option value='2'>Question</option>" &_
                    "<option value='3'>Security</option>" &_
                    "<option value='4'>Citrix</option>" &_
                    "<option value='5'>Apple</option>" &_
                    "<option value='6'>PDA</option>" &_
                    "<option value='7'>SOHO</option>" &_
                    "<option value='8'>Unix-Linux-Cobalt</option>" &_
                    "<option value='9'>Novell</option>" &_
                    "<option value='10'>Net - Enterprise</option>" &_
                    "<option value='11'>Net - SOHO</option>" &_
                    "<option value='12'>Software (Microsoft)</option>" &_
                    "<option value='13'>General PC</option>" &_
                    "<option value='14'>Telephony</option>" &_
                    "<option value='15'>Canadian</option>"

    GetRemedyCaseTypeOptions = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Case Type Value to Case Type Name
'     Output: Value/Null

Function FormatRemedyCaseType(CaseType)
    Dim returnValue
    returnValue = Null

    Select Case CaseType
    Case "1"
        returnValue = "Problem"
    Case "2"
        returnValue = "Question"
    Case "3"
        returnValue = "Security"
    Case "4"
        returnValue = "Citrix"
    Case "5"
        returnValue = "Apple"
    Case "6"
        returnValue = "PDA"
    Case "7"
        returnValue = "SOHO"
    Case "8"
        returnValue = "Unix - Linux - Cobalt"
    Case "9"
        returnValue = "Novell"
    Case "10"
        returnValue = "Net - Enterprise"
    Case "11"
        returnValue = "Net - SOHO"
    Case "12"
        returnValue = "Software (Microsoft)"
    Case "13"
        returnValue = "General PC"
    Case "14"
        returnValue = "Telephony"
    Case "15"
        returnValue = "Canadian"
    End Select  

    FormatRemedyCaseType = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Drop down options for Remedy Operating System select box options
'     Output: HTML Text

Function GetRemedyOsOptions()
    Dim returnValue
    returnValue = Null

    returnValue   = "<option value='1'>None</option>" &_
                    "<option value='2'>Windows XP Pro</option>" &_
                    "<option value='3'>Windows XP Home</option>" &_
                    "<option value='4'>Windows XP Media</option>" &_
                    "<option value='5'>Windows 2000 Server</option>" &_
                    "<option value='6'>Windows 2000 Pro</option>" &_
                    "<option value='7'>Windows 2000</option>" &_
                    "<option value='8'>Windows 2003</option>" &_
                    "<option value='9'>Windows NT</option>" &_
                    "<option value='10'>Windows ME</option>" &_
                    "<option value='11'>Windows 98</option>" &_
                    "<option value='12'>Windows 95</option>" &_
                    "<option value='13'>Windows 3.1</option>" &_
                    "<option value='14'>Solaris</option>" &_
                    "<option value='15'>Dos</option>" &_
                    "<option value='16'>Mac OS X</option>" &_
                    "<option value='17'>Mac 9</option>" &_
                    "<option value='18'>Mac 8.X</option>" &_
                    "<option value='19'>Mac 7.X</option>" &_
                    "<option value='20'>Novell</option>" &_
                    "<option value='21'>Other</option>" &_
                    "<option value='22'>Palm OS</option>" &_
                    "<option value='23'>Windows CE</option>" &_
                    "<option value='24'>Linux</option>" &_
                    "<option value='25'>Blackberry</option>" &_
                    "<option value='26'>HP/UX</option>"

    GetRemedyOsOptions = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     OS Value to OS Name
'     Output: Value/Null

Function FormatRemedyOs(Os)
    Dim returnValue
    returnValue = Null

    Select Case Os
    Case "1"
        returnValue = "None"
    Case "2"
        returnValue = "Windows XP Pro"
    Case "3"
        returnValue = "Windows XP Home"
    Case "4"
        returnValue = "Windows XP Media"
    Case "5"
        returnValue = "Windows 2000 Server"
    Case "6"
        returnValue = "Windows 2000 Pro"
    Case "7"
        returnValue = "Windows 2000"
    Case "8"
        returnValue = "Windows 2003"
    Case "9"
        returnValue = "Windows NT"
    Case "10"
        returnValue = "Windows ME"
    Case "11"
        returnValue = "Windows 98"
    Case "12"
        returnValue = "Windows 95"
    Case "13"
        returnValue = "Windows 3.1"
    Case "14"
        returnValue = "Solaris"
    Case "15"
        returnValue = "Dos"
    Case "16"
        returnValue = "Mac OS X"
    Case "17"
        returnValue = "Mac 9"
    Case "18"
        returnValue = "Mac 8.X"
    Case "19"
        returnValue = "Mac 7.X"
    Case "20"
        returnValue = "Novell"
    Case "21"
        returnValue = "Other"
    Case "22"
        returnValue = "Palm OS"
    Case "23"
        returnValue = "Windows CE"
    Case "24"
        returnValue = "Linux"
    Case "25"
        returnValue = "Blackberry"
    Case "26"
        returnValue = "HP/UX"
    End Select  

    FormatRemedyOs = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Drop down options for Remedy Case Contact select box options
'     Output: HTML Text

Function GetRemedyCaseContactOptions()
    Dim returnValue
    returnValue = Null

    returnValue   = "<option value='1'>Case notes on support.cdw.com</option>" &_
                    "<option value='2'>Email response</option>" &_
                    "<option value='3'>Phone call</option>" &_
                    "<option value='4'>Chat request</option>"

    GetRemedyCaseContactOptions = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Contact Value to Contact type
'     Output: Value/Null

Function FormatRemedyCaseContact(CaseContact)
    Dim returnValue
    returnValue = Null
    
    Select Case CaseContact
    Case "1"
        returnValue = "WEB"
    Case "2"
        returnValue = "EMAIL"
    Case "3"
        returnValue = "PHONE"
    Case "4"
        returnValue = "CHAT"
    End Select    

    FormatRemedyCaseContact = returnValue
End Function

'******************************************************************************************************************************
'  DESCRIPTION:
'     Convert Priority Value to Priority Name
'     Output: Value/Null

Function FormatRemedyCaseActionType(CaseActionType)
    Dim returnValue
    returnValue = Null
    
    Select Case CaseActionType
    Case "1"
        returnValue = "Add Notes"
    Case "2"
        returnValue = "Close Case"
    End Select    

    FormatRemedyCaseActionType = returnValue
End Function
%>
