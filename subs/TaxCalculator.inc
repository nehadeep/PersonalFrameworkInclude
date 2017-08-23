<%
' Tax calculation object.
' 1/9/2004 - Nick Pirocanac

' Requires:
'	XML 3.0
' And these includes:
'	<!-- #include virtual="/global/subs/global.inc"-->
'	<!-- #include virtual="/global/subs/sqlconn.inc"-->
'	<!-- #include virtual="/global/subs/parsevalidate.inc" -->
'	<!-- #include virtual="/global/subs/checkoutsubs.inc"-->
'	<!-- #include virtual="/global/subs/debugsubs.inc"-->

'***************************************************************************************
' This Object is used to represent a tax cost for a particular tax type.
'***************************************************************************************
Class TaxCostObject
	Public TaxType
	Public Amount
	Public Rate
	Public OboAmount
	Public OboRate
End Class

'***************************************************************************************
' This Object is used to represent tax for Freight, Insurance, and Handling.
'***************************************************************************************
Class TaxHeaderObject

	Public aTaxLineItems
	'***************************************************************************************
	Public Property Get TotalTax
		Dim taxTotal, i, oTaxLineItemObject
		taxTotal = 0

		If IsNull(aTaxLineItems) = False And IsEmpty(aTaxLineItems) = False Then
			For i = LBound(aTaxLineItems) to UBound(aTaxLineItems)
				Set oTaxLineItemObject = aTaxLineItems(i)

				taxTotal = taxTotal + oTaxLineItemObject.TaxEDC + oTaxLineItemObject.TaxOBO
			Next
		End If

		TotalTax = taxTotal
	End Property

	'***************************************************************************************
	Public Function AddTaxLineItem(TaxEDC, RateEDC, TaxOBO, RateOBO, TaxType)
		If IsNull(aTaxLineItems) = True Or IsEmpty(aTaxLineItems) Then
			Redim aTaxLineItems(0)
		Else
			Redim Preserve aTaxLineItems(UBound(aTaxLineItems) + 1)
		End If

		Dim oCurrTaxLineItem
		Set oCurrTaxLineItem = New TaxLineItemObject

		oCurrTaxLineItem.TaxType		= TaxType
		oCurrTaxLineItem.TaxEDC			= TaxEDC
		oCurrTaxLineItem.RateEDC		= RateEDC
		oCurrTaxLineItem.TaxOBO			= TaxOBO
		oCurrTaxLineItem.RateOBO		= RateOBO

		Set aTaxLineItems(UBound(aTaxLineItems)) = oCurrTaxLineItem
	End Function

	Public Sub DumpObject()
		Dim i, oTaxLineItemObject
		
		For i = LBound(aTaxLineItems) to UBound(aTaxLineItems)
			Set oTaxLineItemObject = aTaxLineItems(i)
			
			oTaxLineItemObject.DumpObject()
		Next
	End Sub
End Class 


'***************************************************************************************
'***************************************************************************************
Class TaxLineObject 
	Public EDC
	Public Quantity
	Public PreTaxPriceTotal
	Public OrderItemID

	Public aTaxLineItems

	'***************************************************************************************
	Public Property Get TotalTax
		TotalTax = GetTotalTax()
	End Property

	'***************************************************************************************
	Public Function AddTaxLineItem(TaxEDC, RateEDC, TaxOBO, RateOBO, TaxType)

		If IsNull(aTaxLineItems) = True Or IsEmpty(aTaxLineItems) Then
			Redim aTaxLineItems(0)
		Else
			Redim Preserve aTaxLineItems(UBound(aTaxLineItems) + 1)
		End If

		Dim oCurrTaxLineItem
		Set oCurrTaxLineItem = New TaxLineItemObject

		oCurrTaxLineItem.TaxType		= TaxType
		oCurrTaxLineItem.TaxEDC			= TaxEDC
		oCurrTaxLineItem.RateEDC		= RateEDC
		oCurrTaxLineItem.TaxOBO			= TaxOBO
		oCurrTaxLineItem.RateOBO		= RateOBO

		Set aTaxLineItems(UBound(aTaxLineItems)) = oCurrTaxLineItem
	End Function

	'***************************************************************************************
	Public Function GetTotalTax()
		Dim i
		Dim oTaxLineItemObject 
		Dim TotalTax

		TotalTax = 0
		For i = LBound(aTaxLineItems) to UBound(aTaxLineItems)
			Set oTaxLineItemObject = aTaxLineItems(i)

			TotalTax = CDbl(TotalTax) + oTaxLineItemObject.GetTotalTax()
		Next

		GetTotalTax = TotalTax
	End Function

	'***************************************************************************************
	Public Sub DumpObject()
		Dim i
		Dim oTaxLineItemObject 

		Response.Write("OrderItemID: " & OrderItemID & ", ")
		Response.Write("Lines for EDC: " & EDC & ", ")
		Response.Write("Quantity: " & Quantity & ", ")
		Response.Write("PreTaxPriceTotal: " & PreTaxPriceTotal & "<hr><br>")

		For i = LBound(aTaxLineItems) to UBound(aTaxLineItems)
			Set oTaxLineItemObject = aTaxLineItems(i)

			oTaxLineItemObject.DumpObject()
		Next
	End Sub
End Class

'***************************************************************************************
' This object is used to represent the tax for each line item in the tax object.
'***************************************************************************************
Class TaxLineItemObject
	Public TaxEDC
	Public RateEDC
	Public TaxOBO
	Public RateOBO
	Public TaxType

	'***************************************************************************************
	Public Property Get TotalTax
		TotalTax = GetTotalTax()
	End Property
	'***************************************************************************************
	Public Function GetTotalTax()
		Dim i
		Dim oTaxLineItemObject 
		Dim TotalTax

		If IsNull(TaxEDC) Then
			TaxEDC = 0
		End If

		If IsNull(TaxOBO) Then
			TaxOBO = 0
		End If
			
		GetTotalTax = CDbl(TaxEDC) + CDbl(TaxOBO)
	End Function

	'***************************************************************************************
	Public Sub DumpObject()
		Response.Write("<ul><li>TaxEDC: " & TaxEDC & ",</li> ")
		Response.Write("<li>RateEDC: " & RateEDC & ",</li> ")
		Response.Write("<li>TaxOBO: " & TaxOBO & ",</li> ")
		Response.Write("<li>RateOBO: " & RateOBO & ",</li> ")
		Response.Write("<li>TaxType: " & TaxType & "</li></ul><br>")
	End Sub
End Class

'***************************************************************************************
'***************************************************************************************
Class TaxCalculator
	Private mTaxHeader
	'Private dTaxLines
	'Private mTaxLines
	'Private mTaxLineCount
	Private mEnvironmentCodeOverride ' Set this to QA, TS, or PD for testing.
	Private mIsTaxCalculated
	Private mTaxString ' The return message from the AS400 after tax calculation.

	Private aTaxLines
	Private oFreightTax
	Private oHandlingTax 
	Private oInsuranceTax

	Private mIsoCountryCode

	'*****************************************************************************
	' Setup the object
	Private Sub Class_Initialize()
		mTaxHeader = ""

		' Determine the tax environment to use.
		' On As400, the environment code is passed to progam CDWSETENV
		Dim DevelopmentMode
		DevelopmentMode = GetSiteDevelopmentMode()
		Select Case DevelopmentMode
			Case glLocalMode
				mEnvironmentCodeOverride = "QA"
			Case glBetaMode
				mEnvironmentCodeOverride = "TP00"
			Case glQAMode
				mEnvironmentCodeOverride = "QA"
			Case glStagingMode
				mEnvironmentCodeOverride = "QA"
			Case glProductionMode
				'P1 is replacing PD for the failover stuff...
				mEnvironmentCodeOverride = "P1"	
		End Select

		mIsTaxCalculated = False

		' Initialize the tax lines array to nothing, it will be redimmed as items are added.
		aTaxLines = Null

		' Set the freight, handling and insurance tax objects to nothing.
		Set oFreightTax = Nothing
		Set oHandlingTax = Nothing
		Set oInsuranceTax = Nothing
	End Sub

	'*****************************************************************************
	' Teardown the object.
	Private Sub Class_terminate()
		Set oFreightTax = Nothing
		Set oHandlingTax = Nothing
		Set oInsuranceTax = Nothing	
	End Sub

	'-----------------------------------------------------------------------------
	' Property Section
	'-----------------------------------------------------------------------------

	'-----------------------------------------------------------------------------
	' Set this to QA, TS, or PD for testing.
	Public Property Get EnvironmentCodeOverride
		EnvironmentCodeOverride = mEnvironmentCodeOverride
	End Property

	'-----------------------------------------------------------------------------
	Public Property Let EnvironmentCodeOverride(s)
		mEnvironmentCodeOverride = s
		mIsTaxCalculated = False
	End Property


	Private mStateProvince
	
	Public Property Get ShippingStateProvince
		ShippingStateProvince = mStateProvince
	End Property
	
	'-----------------------------------------------------------------------------
	Public Property Get TaxLineCount
		Dim lineCount
		lineCount = 0

		If mIsTaxCalculated = False Then

			If IsNull(aTaxLines) = False And IsEmpty(aTaxLines) = False Then
				lineCount = UBound(aTaxLines)
			Else
				lineCount = 0
			End If

		End If

		TaxLineCount = lineCount
	End Property

	'-----------------------------------------------------------------------------
	Public Property Get TotalEDCTax
		Dim i
		Dim totalTax
		Dim oCurrTaxLine

		totalTax = 0

		If mIsTaxCalculated = True Then

			For i = LBound(aTaxLines) to UBound(aTaxLines)
				Set oCurrTaxLine = aTaxLines(i)
				totalTax = CDbl(totalTax) + CDbl(oCurrTaxLine.GetTotalTax())
			Next

		End If

		TotalEDCTax = totalTax
	End Property

	'-----------------------------------------------------------------------------
	Public Property Get TotalTax
		Dim taxTotal, i, aTaxCost, oTaxCost
		taxTotal = 0

		If mIsTaxCalculated = True And TaxCostCount > 0 Then
			aTaxCost = TaxCostArray

			For i = LBound(aTaxCost) To UBound(aTaxCost)
				Set oTaxCost = aTaxCost(i)

				taxTotal = CDbl(taxTotal) + CDbl(oTaxCost.Amount) + CDbl(oTaxCost.OboAmount)
			Next
		End If

		TotalTax = taxTotal
	End Property

	'-----------------------------------------------------------------------------
	Public Property Get TaxCostCount
		Dim countTaxType, aTaxCost
		countTaxType = 0

		If mIsTaxCalculated = True Then
			aTaxCost = TaxCostArray

			If IsNull(aTaxCost) = False And IsEmpty(aTaxCost) = False Then
				countTaxType = UBound(aTaxCost) + 1
			End If
		End If

		TaxCostCount = countTaxType
	End Property

	'-----------------------------------------------------------------------------
	Public Property Get TaxCostArray
		Dim aTaxCost, i, j, oCurrTaxLine, oCurrTaxLineItem

		If mIsTaxCalculated = True Then
			If IsNull(aTaxLines) = False And IsEmpty(aTaxLines) = False Then
				' Add all the line item tax lines to the bucket array.
				For i = LBound(aTaxLines) to UBound(aTaxLines)
					Set oCurrTaxLine = aTaxLines(i)

					If IsNull(oCurrTaxLine.aTaxLineItems) = False And IsEmpty(oCurrTaxLine.aTaxLineItems) = False then
						For j = LBound(oCurrTaxLine.aTaxLineItems) to UBound(oCurrTaxLine.aTaxLineItems)
							Set oCurrTaxLineItem = oCurrTaxLine.aTaxLineItems(j)
							Call AddTaxLineItemObjectToTaxCostArray(aTaxCost, oCurrTaxLineItem)
						Next
					End If
				Next
			End If

			If Not oFreightTax Is Nothing Then
				If IsNull(oFreightTax.aTaxLineItems) = False And IsEmpty(oFreightTax.aTaxLineItems) = False then
					For j = LBound(oFreightTax.aTaxLineItems) to UBound(oFreightTax.aTaxLineItems)
						Set oCurrTaxLineItem = oFreightTax.aTaxLineItems(j)

						Call AddTaxLineItemObjectToTaxCostArray(aTaxCost, oCurrTaxLineItem)
					Next
				End If
			End If

			If Not oHandlingTax Is Nothing Then
				If IsNull(oHandlingTax.aTaxLineItems) = False And IsEmpty(oHandlingTax.aTaxLineItems) = False then
					For j = LBound(oHandlingTax.aTaxLineItems) to UBound(oHandlingTax.aTaxLineItems)
						Set oCurrTaxLineItem = oHandlingTax.aTaxLineItems(j)

						Call AddTaxLineItemObjectToTaxCostArray(aTaxCost, oCurrTaxLineItem)
					Next
				End If
			End If
			If Not oInsuranceTax Is Nothing Then
				If IsNull(oInsuranceTax.aTaxLineItems) = False And IsEmpty(oInsuranceTax.aTaxLineItems) = False then
					For j = LBound(oInsuranceTax.aTaxLineItems) to UBound(oInsuranceTax.aTaxLineItems)
						Set oCurrTaxLineItem = oInsuranceTax.aTaxLineItems(j)

						Call AddTaxLineItemObjectToTaxCostArray(aTaxCost, oCurrTaxLineItem)
					Next
				End If
			End If
		End IF

		TaxCostArray = aTaxCost
	End Property

	'******************************************************************************
	Public Sub SetTaxHeaderByOrderID(OrderID, CDWSiteCode, CustomerNumber)

		'Response.Write("OrderID: " & OrderID & "<br>")

		' Get the shipping address
		Dim OrderShippingAddress, shipToAddress1, shipToCity, shipToStateProv, shipToPostalCode, shipToISOCountryCode
		OrderShippingAddress = GetCheckoutOrderShippingAddress(OrderID)

		If Not(IsNull(OrderShippingAddress)) and Not(IsEmpty(OrderShippingAddress)) Then
			If IsArray(OrderShippingAddress) Then
				shipToAddress1	  		= Trim(OrderShippingAddress(cShipToAddress1))
				shipToCity		  		= Trim(OrderShippingAddress(cShipToCity))
				shipToStateProv   		= Trim(OrderShippingAddress(cShipToStateProv))
				mStateProvince			= shipToStateProv
				shipToPostalCode  		= Trim(OrderShippingAddress(cShipToPostalCode))
				shipToISOCountryCode	= Trim(OrderShippingAddress(cShipToISOCountryCode))
			End If
		End If

		'Get the ShippingMethod
		Dim OrderShippingMethod, ShippingCharge, HandlingCharge, InsuranceCharge
		OrderShippingMethod = GetCheckoutOrderShippingMethod(OrderID)

		ShippingCharge = 0
		HandlingCharge = 0
		InsuranceCharge = 0
		If IsArray(OrderShippingMethod) Then
			ShippingCharge		= OrderShippingMethod(cGetShippingCharge)
			HandlingCharge		= OrderShippingMethod(cGetHandlingCharge)
			InsuranceCharge		= OrderShippingMethod(cGetInsuranceCharge)
		End If

		Call SetTaxHeader(	shipToAddress1, _
							shipToCity, _
							shipToStateProv, _
							shipToPostalCode, _
							CDWSiteCode, _
							CustomerNumber, _
							HandlingCharge, _
							ShippingCharge, _
							InsuranceCharge, _
							shipToISOCountryCode)
	End Sub

	'*****************************************************************************
	' Add the header elements for calculating tax. This is step 1 for using
	' the TaxCalculator.
	Public Sub SetTaxHeader(	ShipToAddress1, ShipToCity, ShipToStateProv, _
								ShipToPostalCode, CDWSiteCode, CustomerNumber, _
								HandlingCharge, ShippingCharge, InsuranceCharge, ShipToISOCountryCode	)

		mTaxHeader = "env=" & mEnvironmentCodeOverride & "&pgm=WEBTAX"

		' TODO: Remove this when David Stec fixes the tax service to NOT use the 
		' state / province field to determine country.
		'If Trim(ShipToISOCountryCode) = "CA" Then
		'	ShipToStateProv = "CN"
		'End If

		If IsNull(CustomerNumber) Or IsEmpty(CustomerNumber) Or Len(CustomerNumber) = 0 Then
			CustomerNumber = "0"
		End If

		' These are order specific, make sure you maintain this order. Put new items
		' above the handling, freight and insurance values, or you will break the 
		' TaxService on the 400. 
		mTaxHeader = mTaxHeader &_
						"&address="  & ShipToAddress1 &_
						"&city=" & ShipToCity &_
						"&state=" & ShipToStateProv &_
						"&zip=" & ShipToPostalCode&_
						"&country=" & Trim(ShipToISOCountryCode) &_
						"&company=" & CDWSiteCode &_
						"&customer=" & CustomerNumber &_
						"&handling=" & HandlingCharge &_
						"&freight=" & ShippingCharge &_
						"&insurance=" & InsuranceCharge &_
						""

		'Response.Write("mTaxHeader: " & mTaxHeader & "<br>")

		mIsoCountryCode = Trim(ShipToISOCountryCode)
		mIsTaxCalculated = False
	End Sub
	
	'*****************************************************************************
	' Add a single tax line item to the TaxCalculator Object. 
	Public Function AddTaxLineItem(EDC, Quantity, PriceTotal)

		If IsNull(aTaxLines) = True Then
			ReDim aTaxLines(0)
		Else
			ReDim Preserve aTaxLines(UBound(aTaxLines) + 1) 
		End If
		
		Dim newLineItem
		Set newLineItem = new TaxLineObject
		newLineItem.EDC = EDC
		newLineItem.Quantity = Quantity
		newLineItem.PreTaxPriceTotal = PriceTotal
		
		Set aTaxLines(UBound(aTaxLines)) = newLineItem

		mIsTaxCalculated = False
		
		Set AddTaxLineItem = newLineItem
	End Function

	'*****************************************************************************
	' Add all the line items from a passed order id to the TaxCalculator Object.
	Public Sub AddTaxLineItemsByOrderID(OrderID)
		Dim OrderItems, OrderItemEDC, OrderItemPriceTotal, OrderItemQuantity, OrderItemID
		Dim i

		'Get all the items for the passed OrderID
		OrderItems = GetCheckoutOrderItems(OrderID)

		' Add each item to the mTaxLines querystring fragment.
		If(IsArray(OrderItems)) Then
			For i = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
				OrderItemEDC 			= OrderItems(cOrderItemEDC,i)
				OrderItemPriceTotal 	= OrderItems(cOrderItemPriceTotal,i)
				OrderItemQuantity		= OrderItems(cOrderItemQuantity,i)
				OrderItemID				= OrderItems(cOrderItemID,i)

				Dim oNewTaxLineItem 
				Set oNewTaxLineItem = AddTaxLineItem(OrderItemEDC, OrderItemQuantity, OrderItemPriceTotal)
				oNewTaxLineItem.OrderItemID = OrderItemID
				
			Next
		End If

		mIsTaxCalculated = False
	End Sub

	'*****************************************************************************
	' Call the AS400 to calculate the tax.
	public Sub CalculateTax()
		Dim LineItems, i, currLineItem, DevelopmentMode
		LineItems = ""

		'Set currLineItem	= aTaxLines(0)
		DevelopmentMode		= GetSiteDevelopmentMode()
		mIsTaxCalculated	= False

		If IsNull(aTaxLines) = False And IsEmpty(aTaxLines) = False Then
			' create a query string fragment out of all the lines items in the line items array.
			For i = LBound(aTaxLines) to UBound(aTaxLines)
				Set currLineItem = aTaxLines(i)

				LineItems = LineItems & "&line" & i + 1 & "=" & i + 1 & "," & currLineItem.EDC & "," & currLineItem.Quantity & "," & currLineItem.PreTaxPriceTotal		
			Next

			If Len(mTaxHeader) > 0 And Len(LineItems) > 0 Then
				' If the tax call doesn't succeed, then we will just have no tax info.
				If (DevelopmentMode = glProductionMode) OR (DevelopmentMode = glLocalMode) Then
					On Error Resume Next
				End If

				Dim XMLHTTP
				Dim URL
				'Create the XMLHTTP Object. This should be version 3.0. The call could change
				Set XMLHTTP = CreateServerXMLHTTP()

				Select Case DevelopmentMode
					Case glLocalMode
						XMLHTTP.Open "POST", "http://bertha:8081/cgi-bin/http2call", False
					Case glBetaMode
						XMLHTTP.Open "POST", "http://bertha:8081/cgi-bin/http2call", False
					Case glQAMode
						XMLHTTP.Open "POST", "http://bertha:8081/cgi-bin/http2call", False
					Case glStagingMode
						XMLHTTP.Open "POST", "http://bertha:8081/cgi-bin/http2call", False
					Case glProductionMode
						XMLHTTP.Open "POST", "http://as400-tax:8080/cgi-bin/http2call", False
					Case glPreviewMode
						XMLHTTP.Open "POST", "http://as400-tax:8080/cgi-bin/http2call", False
					Case Else
						ErrorRase(errGeneral)
						ErrorRaseSub(erTaxCalculatorNotSetup)
						Response.Redirect Request.Path & GetErrorQueryString()
				End Select

				XMLHTTP.setRequestHeader "Content-Type", "application/x-www-form-urlencoded"

				mTaxString = ""
				mIsTaxCalculated = False

				Dim XmlHttpStatusMessage
				XmlHttpStatusMessage = 0

				Dim responseText
				responseText = "An error occurred getting the XML response."

				On Error Resume Next
					'Testing information for Bill Schalck...Do Not DELETE:  (Only comment Out)
					'	XMLHTTP.Send("env=P1&pgm=WEBTAX&address=5117 E Lake Shore Dr&city=Wonder Lake&state=IL&zip=60097&country=US&company=01&customer= 0624460&handling=6.25&freight=2.25&insurance=1.11&line1=1,123451,1,125.55")
					'	Response.write("XMLHTTP.Send:<br />(env=P1&pgm=WEBTAX&address=5117 E Lake Shore Dr&city=Wonder Lake&state=IL&zip=60097&country=US&company=01&customer= 0624460&handling=6.25&freight=2.25&insurance=1.11&line1=1,123451,1,125.55)<hr />")
					'Real Calls
						XMLHTTP.Send(mTaxHeader & LineItems)
					'	Response.write("XMLHTTP.Send:<br />(" & mTaxHeader & LineItems & ")<hr />")

					XmlHttpStatusMessage = XMLHTTP.Status
					responseText = XMLHTTP.responseText
				On Error Goto 0

'				Response.write("XMLHTTP.Status:  " & XmlHttpStatusMessage & "<hr />")
'				Response.write("XMLHTTP.responseText:  " & responseText & "<hr />")
'				Response.end()

				' The Status will return with a 403 if it couldnt get to the AS/400.
				If (XmlHttpStatusMessage = 200) Then
					ProcessTaxMessage(XMLHTTP.responseText)
					mIsTaxCalculated = True
				End If

				If DevelopmentMode = glProductionMode Then
					on error goto 0
				End If

				' TODO: Remove after debugging.
				'mIsTaxCalculated = False

				If mIsTaxCalculated = False Then
					Dim CurrentSite, CurrentPage, IpAddress, UserAgent, ErrorLogID, ScriptName
					Dim UserKey, UserID, OrganizationID, Key, ServerVariables

					For Each Key In Request.ServerVariables
		 				ServerVariables = ServerVariables & Key & " = " & Request.ServerVariables(Key) & "<br>"
					Next

					UserKey = GetUserKey()
					UserID = GetUserID(UserKey)
					OrganizationID = GetUserOrganizationID(UserID)

					'Log error

					CurrentSite	= GetThisSite()
					CurrentPage	= Request.ServerVariables("SCRIPT_NAME")
					IpAddress	= Request.ServerVariables("REMOTE_ADDR")
					UserAgent	= Request.ServerVariables("HTTP_USER_AGENT")
					ScriptName	= Request.ServerVariables("SCRIPT_NAME")

					Call AddErrorLog(CurrentSite, CurrentPage, UserID, OrganizationID, IPAddress, UserAgent, 666, 666, "TaxCalculator.inc", "TaxCalculator", "/global/subs/TaxCalculator.inc", 0, 0, "There was an error calculating the tax for this user's order. This can happen if the Tax Server is down, or if there is a network interruption between the web server and the Tax Server.", "XmlHttpStatusMessage: " & XmlHttpStatusMessage & vbcrlf & vbcrlf & responseText, ServerVariables)
					Call SendErrorEmailNotification()
				End If

				XMLHTTP = Null
				Set XMLHTTP = Nothing
			Else
				ErrorRase(errGeneral)
				ErrorRaseSub(erTaxCalculatorNotSetup)
				Response.Redirect Request.Path & GetErrorQueryString()
			End If
		End If
	End Sub

	'*****************************************************************************
	' Realign the returned tax info back onto the line items.
	Private Sub ProcessTaxMessage(TaxString)
		'Response.Write(GetSiteDevelopmentMode())
		'Response.Write("Returned TaxMessage: " & TaxString & "<br><br>")
		'Response.End()

		'Variables utilized in getting the values from the 400.
		Dim EDCTaxString
		Dim returnValue

		'Start Formatting the Response
		If Not(IsNull(TaxString)) and Not(IsEmpty(TaxString)) and Len(TaxString) > 0 Then
			TaxString = Replace(TaxString,Chr(10)," ")

			'Lets Split this into its first Array
			Dim ParseArray1, ParseArray2
			Dim Counter, Counter2, TaxLineCounter
			Dim oCurrTaxLine, oCurrTaxLineItem
			Dim foundMatchingEdc
			Dim LineNumber, TaxType, TaxEDC, RateEDC, TaxOBO, RateOBO, EDC

			ParseArray1 = Split(TaxString," " )

			'Loop through the entire name value pair string inside this array
			For Counter = LBOUND(ParseArray1) to UBOUND(ParseArray1)
				'Split the Initial Array into another array
				ParseArray2 = Split(ParseArray1(Counter),"=")

				For Counter2 = LBOUND(ParseArray2) to UBOUND(ParseArray2)
					If (UCASE(ParseArray2(Counter2)) = "TAXHANDLING") Then
						Dim TaxHandlingArray

						If oHandlingTax Is Nothing Then
							Set oHandlingTax = New TaxHeaderObject
						End If

						TaxHandlingArray 	= Split(ParseArray2(Counter2+1),",")

						TaxType		= TaxHandlingArray(0)
						TaxEDC		= TaxHandlingArray(1)
						RateEDC		= TaxHandlingArray(2)
						TaxOBO		= TaxHandlingArray(3)
						RateOBO		= TaxHandlingArray(4)

						Call oHandlingTax.AddTaxLineItem(TaxEDC, RateEDC, TaxOBO, RateOBO, TaxType)

					ElseIf (UCASE(ParseArray2(Counter2)) = "TAXFREIGHT") Then
						Dim TaxFreightArray

						If oFreightTax Is Nothing Then
							Set oFreightTax = New TaxHeaderObject
						End If

						TaxFreightArray 	= Split(ParseArray2(Counter2+1),",")

						TaxType		= TaxFreightArray(0)
						TaxEDC		= TaxFreightArray(1)
						RateEDC		= TaxFreightArray(2)
						TaxOBO		= TaxFreightArray(3)
						RateOBO		= TaxFreightArray(4)

						Call oFreightTax.AddTaxLineItem(TaxEDC, RateEDC, TaxOBO, RateOBO, TaxType)
					ElseIf (UCASE(ParseArray2(Counter2)) = "TAXINSURANCE") Then
						Dim TaxInsuranceArray

						If oInsuranceTax Is Nothing Then
							Set oInsuranceTax = New TaxHeaderObject
						End If

						TaxInsuranceArray 			= Split(ParseArray2(Counter2+1),",")

						TaxType		= TaxInsuranceArray(0)
						TaxEDC		= TaxInsuranceArray(1)
						RateEDC		= TaxInsuranceArray(2)
						TaxOBO		= TaxInsuranceArray(3)
						RateOBO		= TaxInsuranceArray(4)

						Call oInsuranceTax.AddTaxLineItem(TaxEDC, RateEDC, TaxOBO, RateOBO, TaxType)
					ElseIf InStr(ParseArray2(Counter2),"taxline") Then
						EDCTaxString = Split(ParseArray2(Counter2+1),",")
						LineNumber	= EDCTaxString(0)
						EDC			= EDCTaxString(1)
						TaxType		= EDCTaxString(2)
						TaxEDC		= EDCTaxString(3)
						RateEDC		= EDCTaxString(4)
						TaxOBO		= EDCTaxString(5)
						RateOBO		= EDCTaxString(6)

						Set oCurrTaxLine = aTaxLines(LineNumber - 1)

						Call oCurrTaxLine.AddTaxLineItem(TaxEDC, RateEDC, TaxOBO, RateOBO, TaxType)
					End If
				Next
			Next
		End If
	End Sub

	'**************************************************************************************
	' Returns an array of tax information that can be passed to the shipping calculator.
	Public Function GetShippingTaxMessage(OrderID)

		Dim returnValue
		
		returnValue = Null

		If mIsTaxCalculated = True Then
			Dim ShippingTaxMessage(7)
			
			ShippingTaxMessage(TaxInfoOrderID) 			= OrderID
			ShippingTaxMessage(TaxInfoShipTax) 			= oFreightTax.TaxCost
			ShippingTaxMessage(TaxInfoShipTaxRate) 		= oFreightTax.TaxRate
			ShippingTaxMessage(TaxInfoHandlingTax) 		= oHandlingTax.TaxCost
			ShippingTaxMessage(TaxInfoHandlingTaxRate) 	= oHandlingTax.TaxRate
			ShippingTaxMessage(TaxInfoInsuranceTax) 	= oInsuranceTax.TaxCost
			ShippingTaxMessage(TaxInfoInsuranceTaxRate) = oInsuranceTax.TaxRate
			ShippingTaxMessage(TaxInfoTotalEDCTax)		= TotalTax

			returnValue = ShippingTaxMessage
		End If
		
		GetShippingTaxMessage = returnValue

	End Function
	
	
	'**************************************************************************************
	' Updates the database with the shipping tax information for the passed order id.
	Public Function UpdateShippingTaxInformation(OrderID)
		Dim oTaxLineItem
		Dim UpdateSuccess, OrderItemTaxTypeID, i
			
		' Delete any previous existing tax info for the order. In the future, some
		' countries may have multiple different taxes on shipping.
		UpdateSuccess = newCheckout_DeleteOrderTaxes(OrderID)

		If mIsTaxCalculated = True Then
			If oFreightTax.TotalTax > 0 Then
				UpdateSuccess = UpdateOrderTaxHeaderInformation(OrderID, "FREIGHT", oFreightTax)
			End If
			
			If oHandlingTax.TotalTax > 0 Then
				UpdateSuccess = UpdateOrderTaxHeaderInformation(OrderID, "HANDLING", oHandlingTax)
			End If
			
			If oInsuranceTax.TotalTax > 0 Then
				UpdateSuccess = UpdateOrderTaxHeaderInformation(OrderID, "INSURANCE", oInsuranceTax)
			End If
		End If
		
	End Function 
	
	'*****************************************************************************
	Private Function UpdateOrderTaxHeaderInformation(OrderID, TaxCategory, oTaxHeaderObject)
		Dim UpdateSuccess, OrderItemTaxTypeID, i, oTaxLineItem
		
		UpdateSuccess = False
		
		'Response.Write ("mIsTaxCalculated: " & mIsTaxCalculated)
	
		' Send up the tax for the frieght lines
		If mIsTaxCalculated = True And IsNull(oTaxHeaderObject) = False And IsEmpty(oTaxHeaderObject) = False Then

			If IsNull(oTaxHeaderObject.aTaxLineItems) = False And IsEmpty(oTaxHeaderObject.aTaxLineItems) = False Then
				For i = LBound(oTaxHeaderObject.aTaxLineItems) To UBound(oTaxHeaderObject.aTaxLineItems) 
					Set oTaxLineItem = oTaxHeaderObject.aTaxLineItems(i)
					
					If oTaxLineItem.TotalTax > 0 Then
					
						'Response.Write("UpdateOrderTaxHeaderInformation: " & oTaxLineItem.TaxType & "<br>")
						
						OrderItemTaxTypeID = newCheckout_GetOrderItemTaxTypeIDByName(TaxCategory & "_" & oTaxLineItem.TaxType, mIsoCountryCode)
				
						If IsNull(OrderItemTaxTypeID) = False And IsEmpty(OrderItemTaxTypeID) = False Then
						
							UpdateSuccess = newCheckout_UpsertOrderItemTaxes( _
														OrderID, _
														Null, _
														OrderItemTaxTypeID, _
														oTaxLineItem.RateEDC, _
														oTaxLineItem.TaxEDC, _
														oTaxLineItem.RateOBO, _
														oTaxLineItem.TaxOBO _
													)			
						End If
					End If
				Next
			End If		
		End If
		
		
		

		UpdateOrderTaxHeaderInformation = UpdateSuccess
	End Function
	

	'*****************************************************************************
	Public Sub UpdateTaxLineItems(OrderID)
	
		Dim TaxLineCounter, oCurrTaxLine, UpdateSuccess, TaxLineItemCounter, oCurrTaxLineItem

		' Delete the taxes for the lines, so that if the user changes countries, they
		' will still have the right information.
		newCheckout_DeleteOrderItemTaxes( OrderID )
	
		If mIsTaxCalculated = True Then
	
			For TaxLineCounter = LBound(aTaxLines) To UBound(aTaxLines)
				Set oCurrTaxLine = aTaxLines(TaxLineCounter)
				
				For TaxLineItemCounter = LBound(oCurrTaxLine.aTaxLineItems) To UBound(oCurrTaxLine.aTaxLineItems)
					Set oCurrTaxLineItem = 	oCurrTaxLine.aTaxLineItems(TaxLineItemCounter)
				
					If oCurrTaxLineItem.TotalTax > 0 Then
						
						' Get the tax type id for the current line.
						Dim OrderItemTaxTypeID
						OrderItemTaxTypeID = newCheckout_GetOrderItemTaxTypeIDByName(oCurrTaxLineItem.TaxType, mIsoCountryCode)
					
					
						'Response.Write("oCurrTaxLineItem.TaxType: " & oCurrTaxLineItem.TaxType & "<br>")
						'Response.Write("mIsoCountryCode: " & mIsoCountryCode & "<br>")
						'Response.Write("OrderItemTaxTypeID: " & OrderItemTaxTypeID & "<br>")			
						'Response.Write ("test:" &  orderid & "<BR>")
						'Response.Write ("test:" &  oCurrTaxLine.OrderItemID & "<BR>")
						'Response.Write ("test:" &  OrderItemTaxTypeID & "<BR>")
						'Response.Write ("test:" &  oCurrTaxLineItem.RateEDC & "<BR>")
						'Response.Write ("test:" &  oCurrTaxLineItem.TaxEDC & "<BR>")
						'Response.Write ("test:" &  oCurrTaxLineItem.RateOBO  & "<BR>")
						'Response.Write ("test:" &  oCurrTaxLineItem.TaxOBO & "<BR>")
						'Response.Write ("test:" &  oCurrTaxLineItem.TaxOBO & "<BR>")
						'Response.End					
						
						UpdateSuccess = newCheckout_UpsertOrderItemTaxes( _
											OrderID, _
											oCurrTaxLine.OrderItemID, _
											OrderItemTaxTypeID, _
											oCurrTaxLineItem.RateEDC, _
											oCurrTaxLineItem.TaxEDC, _
											oCurrTaxLineItem.RateOBO, _
											oCurrTaxLineItem.TaxOBO _
										)
										

					
						If UpdateSuccess = False Then
							Err.Raise 1,GetThisPage(),"TaxCalculator: There was an error updating your OrderItems"
						End If
						
					End If 'If oCurrTaxLineItem.TotalTax > 0 Then
					
				Next
			Next

		Else
		
			' Mark the order's tax info as being in error, if the tax wasn't calculated.
			OrderItemTaxTypeID = newCheckout_GetOrderItemTaxTypeIDByName("ERROR", Null)
	
			If IsNull(OrderItemTaxTypeID) = False And IsEmpty(OrderItemTaxTypeID) = False Then
			
				UpdateSuccess = newCheckout_UpsertOrderItemTaxes( _
											OrderID, _
											Null, _
											OrderItemTaxTypeID, _
											0, _
											0, _
											0, _
											0 _
										)
										
										'Response.Write(OrderID & " " & OrderItemTaxTypeID & "<br>")
										'Response.End
										
			End If
		End If

	End Sub
	
	
	'*****************************************************************************
	Public Function GetTaxLineObject(Index)
		Dim returnValue
		Set returnValue = Nothing
	
		If IsNull(aTaxLines) = False And IsEmpty(aTaxLines) = False Then
			If Index >= LBound(aTaxLines) And Index <= UBound(aTaxLines) Then
				Set returnValue = aTaxLines(Index)
				
			End If
		End If
	
		Set GetTaxLineObject = returnValue
	
	End Function
	
	
	
	'***************************************************************************************
	' Add the passed tax line to the passed tax cost array.
	Private Function AddTaxLineItemObjectToTaxCostArray(aTaxCost, oCurrTaxLineItem)
		Dim i, j, hasMatch
		
		hasMatch = False
		
		' If the aTaxCost exists, then look through it for an existing bucket for 
		' this tax type.
		If IsNull(aTaxCost) = False And IsEmpty(aTaxCost) = False Then
			Dim oTaxCostItem
			
			For j = LBound(aTaxCost) to UBound(aTaxCost)
				Set oTaxCostItem = aTaxCost(j)
		
				If oTaxCostItem.TaxType = oCurrTaxLineItem.TaxType Then
					oTaxCostItem.Amount = CDbl(oTaxCostItem.Amount) + CDbl(oCurrTaxLineItem.TaxEDC)
					oTaxCostItem.OboAmount = CDbl(oTaxCostItem.OboAmount) + CDbl(oCurrTaxLineItem.TaxOBO)
					
					'Response.Write("Added: " & oCurrTaxLineItem.TaxType & ", " & oCurrTaxLineItem.TaxEDC & ", " & oCurrTaxLineItem.TaxOBO & "<br>")
					hasMatch = True
					Exit For
				End If
			Next
		End If
		
		' If no tax type was already in the array, then add a new bucket for it.
		' We are going to assume that once the rate is set, then the rate will not change,
		' so that if another TaxLineItem of the same tax type is added, then the rate will
		' stay the same.
		If hasMatch = False Then
		
			If IsNull(aTaxCost) = False And IsEmpty(aTaxCost) = False Then
				Redim Preserve aTaxCost(UBound(aTaxCost) + 1)
			Else
				Redim aTaxCost(0)
			End If
				
			Dim oNewTaxCost
			Set oNewTaxCost = new TaxCostObject
			
			oNewTaxCost.TaxType = oCurrTaxLineItem.TaxType
			oNewTaxCost.Amount = oCurrTaxLineItem.TaxEDC
			oNewTaxCost.Rate = oCurrTaxLineItem.RateEDC
			oNewTaxCost.OboAmount = oCurrTaxLineItem.TaxOBO
			oNewTaxCost.OboRate = oCurrTaxLineItem.RateOBO
			
			Set aTaxCost(UBound(aTaxCost)) = oNewTaxCost
			
			'Response.Write("Created: " & oCurrTaxLineItem.TaxType & ", " & oCurrTaxLineItem.TaxEDC & ", " & oCurrTaxLineItem.TaxOBO & "<br>")
		End If
		
		AddTaxLineItemObjectToTaxCostArray = aTaxCost
	
	End Function
	
	
	
	Public Sub DumpObject
		Dim lineCounter, currLine

		Response.Write("<br><br>oFreightTax: ")
		oFreightTax.DumpObject()

		Response.Write("oHandlingTax: ")
		oHandlingTax.DumpObject()

		Response.Write("oInsuranceTax: ")
		oInsuranceTax.DumpObject()

		Response.Write("Line Items<hr>")
		For lineCounter = 0 To TaxLineCount
			Set currLine = GetTaxLineObject(lineCounter)
			
			currLine.DumpObject()
		Next

		Response.Write("TotalEDCTax: " & TotalEDCTax & "<br>")
		
		
		Response.Write("Tax Type Count: " & oTaxCalculator.TaxCostCount & "<br><hr>")
	
		If oTaxCalculator.TaxCostCount > 0 Then
			Dim aTaxCost, i, oTaxCost
			aTaxCost = oTaxCalculator.TaxCostArray
			
			For i = LBound(aTaxCost) To UBound(aTaxCost)
				Set oTaxCost = aTaxCost(i)
				
				Response.Write("Tax: " & oTaxCost.TaxType & ", " & oTaxCost.Amount + oTaxCost.OboAmount & "<br>")
			
			Next
		End If
		
		Response.Write("Total Tax: " & oTaxCalculator.TotalTax & "<br>")
	End Sub

End Class


'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
' Debugging goodness, please don't remove.
'XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
Function DebugCalculateDemoTax()

	Dim oTaxCalculator

	Set oTaxCalculator = New TaxCalculator

	oTaxCalculator.SetTaxHeader "7534 Elwell St.", "Burnaby", "ON", "V5E1L6", "17", "", "10", "20", "10", "CA"
	'oTaxCalculator.AddTaxLineItemsByOrderID 6265033
	'oTaxCalculator.AddTaxLineItem "123451", "1", "20000.00"
	oTaxCalculator.AddTaxLineItemsByOrderID(89)
	oTaxCalculator.CalculateTax

	oTaxCalculator.DumpObject
	
	oTaxCalculator.UpdateShippingTaxInformation(89)
	oTaxCalculator.UpdateTaxLineItems(89)
	
	
	
	
End Function

' Uncomment this line, and rename the page to TaxCalculator.asp to debug it.
'DebugCalculateDemoTax()

%>