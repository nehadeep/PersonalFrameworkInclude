<%
	'	DESCRIPTION:
	'		Functions for batch jobs
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc

	' Batch Job Constants
	Const jbStopped	= False
	Const jbRunning	= True

	Const bjCDW_Departments									= "DBBA4067D8E111D3B6FF0008C7C55D36"	' Add/Update CDW_Departments Table.
	Const bjCDW_Employees									= "DBBA4164D8E111D3B6FF0008C7C55D36"	' Add/Update CDW_Employees Table.
	Const bjCDW_ExEmployees									= "ACE7E6E9F20C11D3B7010008C7C55D36"	' Delete Ex Employees from CDW_Employees Table.
	Const bjCDW_Manufactures								= "4C3597CC327411D4B7050008C7C55D36"	' Add/Update CDW Manufactures.
	Const bjEaccount_OrganizationNames						= "E85FF87F21E011D4B7040008C7C55D36"	' Update Organization Names.
	Const bjEaccount_OrganizationCompanies					= "E85FF8C121E011D4B7040008C7C55D36"	' Update Organization Companies.
	Const bjEaccount_OrganizationEmployees					= "E85FF8C721E011D4B7040008C7C55D36"	' Update Organization Employees.
	Const bjEaccount_OrganizationExtranets					= "E85FF8D221E011D4B7040008C7C55D36"	' Update Organization Extranets.
	Const bjEaccount_OrganizationPlatinums					= "E85FF8D921E011D4B7040008C7C55D36"	' Update Organization Platinums.
	Const bjEaccount_OrganizationBillingAddresses			= "DBBA4FC4D8E111D3B6FF0008C7C55D36"	' Add/Update Eaccount_OrganizationBillingAddresses Table.
	Const bjEaccount_OrganizationShippingAddresses			= "DBBA4FC9D8E111D3B6FF0008C7C55D36"	' Add/Update Eaccount_OrganizationShippingAddresses Table.
	Const bjSavedCart_SendReminder							= "B254091DE87E11D3B6FF0008C7C55D36"	' Send Email Reminder.
	Const bjSavedCart_DeleteOldCarts						= "B2540A9CE87E11D3B6FF0008C7C55D36"	' Delete Old Saved Carts.
	Const bjSearch_BuildMfgList								= "ACE7FBA6F20C11D3B7010008C7C55D36"	' Build manufacture list for search.
	Const bjCDWG_BuildContractLists							= "F22DE58D0F2911D4B7030008C7C55D36"	' Build contract lists for CDWG.
	Const bjProduct_Products								= "46CA8E8037FD11D4B7050008C7C55D36"	' Add/Update Product_Products Table.
	Const bjProduct_ProductPricing							= "4BD371733D8F11D4B7060008C7C55D36"	' Update Product Pricing.
	Const bjProduct_ProductAccessories						= "B11D9A4D5A9411D4B70A0008C7C55D36"	' Add Product Accessories
	Const bjProduct_ProductBundles							= "B11D9DBF5A9411D4B70A0008C7C55D36"	' Add Product Bundles
	Const bjProduct_ModifiedProductPricing					= "72A3D450413611D4B7060008C7C55D36"	' Update Modified Product Pricing.
	Const bjProduct_NightlyModifiedProductPricing			= "5B92473994EB4B648241DC2796DC88BE"	' Update Product Pricing Nightly.
	Const bjProduct_ProductInventories						= "72A3BF25413611D4B7060008C7C55D36"	' Update Product Inventories.
	Const bjProduct_ModifiedProductInventories				= "72A3D582413611D4B7060008C7C55D36"	' Update Modified Product Inventories.
	Const bjProduct_ProductDistributorInventories			= "72A3D169413611D4B7060008C7C55D36"	' Update Product Distributor Inventories.
	Const bjProduct_ModifiedProductDistributorInventories	= "72A3D584413611D4B7060008C7C55D36"	' Update Modified Product Distributor Inventories.
	Const bjProduct_CDWCatalogSchema						= "579551FB513D11D4B7080008C7C55D36"	' Maintain CDW.com Catalog Schema.
	Const bjProduct_CDWGCatalogSchema						= "73C60428A5F411D4B70C0008C7C55D36"	' Maintain CDWG.com Catalog Schema.
	Const bjDirectory_MasterDirectory						= "579B971530AD11D4B7050008C7C55D36"	' Add/Update Master Directory.
	Const bjDirectory_MasterDirectoryFolderAssignments		= "0135D7443E4111D4B7060008C7C55D36"	' Add master directory product folder assignments.
	Const bjEaccount_OrganizationMultipleBillingAddresses   = "DBBE4FC4D8E111D3B6FF0008C7C55D36"	' Add multiple billing addresses
	Const bjAttribute_MoveToCompleted					    = "56BA4CF366F211D4B70B0008C7C55D36"	' Move Edited Attributes to Completed
	Const bjAttribute_SyncLiveData							= "140133237DE511D4B70B0008C7C55D36"	' Update Live Attributes from Old Database
	const bjSiteMerch_ExclusivesXMLGenerator				= "EF21534BBDD411D4B70C0008C7C55D36"    ' Generate XML exclusives files
	const bjProductFinder_Generate							= "B88E99D9C7D411D4B70D0008C7C55D36"	' Generate Product Finders
	const bjEaccount_UserOrganizationLink					= "96F161B76F164DA1B2CAD38FAFFFA292"	' User organization links
	const bjProduct_CopyImages								= "867F58CC980342C59C740AD9ACCF6912"	' Copy product images and thumbs
	const bjOrderUpload_CheckStatus							= "734B7B68284342B6A9780F443AE3FD6A"	' Verfiy that order upload is running
	const bjQueue_QueueUpload								= "68ED97070DFD4BEDADF6C51E4C8B824A"	' upload db table messages to MQSeries queue
	Const bjReports_ReportDistribution						= "4D23C52497FA42BC9409179B2D94BD75"	' Process Report files.
	Const bjReports_ReportSchedules							= "1B7926EDB2F14637802EDBFC5083DF85"	' Process Report schedules.
	Const bjWebData_BuildProjects							= "7BC72FB7BBD64DC5AB9490ECBF1F9F8B"	' Builds software projects.
	Const bjEaccount_BuyersAlerts							= "19DDE88641B447B6BF834820A0829983"	' Sends out Buyer Alert Emails
	Const bjCNetGeneralReporting							= "AB895BDE0E534280B3D18186606385A7"

'***************************************************************
'	DESCRIPTION:
'		Starts a batch job.
'		JobExecutionHistoryID/Null

Function JobStart(JobID)
	Dim returnValue, Job, JobExecutionHistoryID, JobExecutionHistory
	JobExecutionHistoryID = AddJobExecutionHistory(JobID, jbRunning, Now(), Null, Null, Null, Null, Null)
	Job = GetJob(JobID)
	JobExecutionHistory = GetJobExecutionHistory(JobExecutionHistoryID)
	If(Not(IsNull(Job))) Then
		Response.Write("----------" & vbCrLf)
		Response.Write("Start" & vbCrLf)
		Response.Write("----------" & vbCrLf)	
		Response.Write(" Job Name............" & Job(bjJobName) & vbCrLf)
		Response.Write(" File Name..........." & Job(bjFileName) & vbCrLf)
		Response.Write(" Run Every..........." & Job(bjRunEvery) & " Hours" & vbCrLf)
		If(Not(IsNull(JobExecutionHistory))) Then
			Response.Write(" Date Started........" & JobExecutionHistory(bjJobExecutionHistoryDateStarted) & vbCrLf)
		End If
		Response.Write("----------" & vbCrLf)	
		Response.Write("Working..." & vbCrLf)
	End If
	JobStart = JobExecutionHistoryID
End Function


'***************************************************************
'	DESCRIPTION:
'		Finishes a batch job.
'		Error True/False

Function JobFinish(JobID, JobExecutionHistoryID)
	Dim returnValue, JobExecutionHistory
	returnValue = SetJobExecutionHistoryState(JobExecutionHistoryID, jbStopped)
	returnValue = SetJobExecutionHistoryDateFinished(JobExecutionHistoryID, Now())
	JobExecutionHistory = GetJobExecutionHistory(JobExecutionHistoryID)
	If(Not(IsNull(JobExecutionHistory))) Then
		Response.Write("----------" & vbCrLf)
		Response.Write(" Date Finished......." & JobExecutionHistory(bjJobExecutionHistoryDateFinished) & vbCrLf)
		Response.Write(" Execution Time......" & DateDiff("s", JobExecutionHistory(bjJobExecutionHistoryDateStarted), JobExecutionHistory(bjJobExecutionHistoryDateFinished)) & vbCrLf)
		If(Not(IsNull(JobExecutionHistory(bjJobExecutionHistoryErrorNumber)))) Then
			Response.Write(" Error Number........" & JobExecutionHistory(bjJobExecutionHistoryErrorNumber) & vbCrLf)
			Response.Write(" Error Source........" & JobExecutionHistory(bjJobExecutionHistoryErrorSource) & vbCrLf)
			Response.Write(" Error Description..." & JobExecutionHistory(bjJobExecutionHistoryErrorDescription) & vbCrLf)
		End If
		Response.Write("----------" & vbCrLf)
		Response.Write("End" & vbCrLf)
		Response.Write("----------" & vbCrLf)
	End If
	JobFinish = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Handles a job error.
'		Error True/False

Function JobErrorHandler(JobExecutionHistoryID)
	Dim returnValue
	returnValue = SetJobExecutionHistoryError(JobExecutionHistoryID, Err.Number, Err.Source, Err.Description)
	JobErrorHandler = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns batch job information.
'		Array/Null

Const bjJobID				= 0
Const bjJobKey				= 1
Const bjJobName				= 2
Const bjJobDescription		= 3
Const bjFileName			= 4
Const bjRunEvery			= 5
Const bjDisabled			= 6
Const bjDateCreated			= 7
Const bjDateModified		= 8

Function GetJob(JobID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(JobID) and Not IsNull(JobID) and IsNumeric(JobID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_GetJob")
		Cmd.Parameters.Append Cmd.CreateParameter("@JobID", adInteger, adParamInput,, JobID)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@JobKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)		'4
		Cmd.Parameters.Append Cmd.CreateParameter("@FileName", adVarChar, adParamOutput, 100)			'5
		Cmd.Parameters.Append Cmd.CreateParameter("@RunEvery", adInteger, adParamOutput)				'6
		Cmd.Parameters.Append Cmd.CreateParameter("@Disabled", adBoolean, adParamOutput)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)			'8
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)		'9
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			Dim ary(8)
			ary(bjJobID)				= JobID
			ary(bjJobKey)				= Cmd.Parameters(2).Value
			ary(bjJobName)				= Cmd.Parameters(3).Value
			ary(bjJobDescription)		= Cmd.Parameters(4).Value
			ary(bjFileName)				= Cmd.Parameters(5).Value
			ary(bjRunEvery)				= Cmd.Parameters(6).Value
			ary(bjDisabled)				= Cmd.Parameters(7).Value
			ary(bjDateCreated)			= Cmd.Parameters(8).Value
			ary(bjDateModified)			= Cmd.Parameters(9).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetJob = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all batch jobs.
'		Note: Constants defined above
'		Array/Null

Function GetJobs()
	Dim returnValue, Cmd, resultSet
	returnValue = Null
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_GetJobs")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	GetJobs = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns batch jobs based on Criteria Search.
'		Note: Constants defined above
'		Array/Null

Function JobsSearch(CriteriaType,Criteria)
	Dim returnValue, Cmd, resultSet
	returnValue = Null
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_JobsSearch")
	Cmd.Parameters.Append Cmd.CreateParameter("@CriteriaType", adInteger, adParamInput, 4, CriteriaType)
	Cmd.Parameters.Append Cmd.CreateParameter("@Criteria", adVarChar, adParamInput, 75, Criteria)
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If resultSet.State = adstateOpen Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	JobsSearch = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets the Job id associated with the Job key.
'		Returns JobID/Null

Function GetJobID(JobKey)
	Dim returnValue, Cmd
	returnValue = Null
	If Not IsEmpty(JobKey) and Not IsNull(JobKey) and Len(JobKey) > 0 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_GetJobID")
		Cmd.Parameters.Append Cmd.CreateParameter("@JobKey", adVarChar, adParamInput, 32, JobKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@JobID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetJobID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds job execution history.
'		Returns JobExecutionHistoryID/Null

Function AddJobExecutionHistory(JobID, State, DateStarted, DateFinished, Note, ErrorNumber, ErrorSource, ErrorDescription)
	Dim returnValue, Cmd
	returnValue = Null
		If Not IsEmpty(JobID) and Not IsNull(JobID) and IsNumeric(JobID) Then
			If Not IsEmpty(State) and Not IsNull(State) and IsNumeric(State) Then
				If Not Len(ErrorSource) > 0 Then ErrorSource = Null End If
				If Not Len(ErrorDescription) > 0 Then ErrorDescription = Null End If
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_AddJobExecutionHistory")
				Cmd.Parameters.Append Cmd.CreateParameter("@JobID", adInteger, adParamInput,, JobID)							'1
				Cmd.Parameters.Append Cmd.CreateParameter("@State", adBoolean, adParamInput,, State)							'2
				Cmd.Parameters.Append Cmd.CreateParameter("@DateStarted", adDBTimeStamp, adParamInput,, DateStarted)			'3
				Cmd.Parameters.Append Cmd.CreateParameter("@DateFinished", adDBTimeStamp, adParamInput,, DateFinished)			'4
				Cmd.Parameters.Append Cmd.CreateParameter("@Note", adVarChar, adParamInput, 500, Note)							'5
				Cmd.Parameters.Append Cmd.CreateParameter("@ErrorNumber", adInteger, adParamInput,, ErrorNumber)				'6
				Cmd.Parameters.Append Cmd.CreateParameter("@ErrorSource", adVarChar, adParamInput, 100, ErrorSource)			'7
				Cmd.Parameters.Append Cmd.CreateParameter("@ErrorDescription", adVarChar, adParamInput, 100, ErrorDescription)	'8
				Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamOutput)					'9
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(9).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	AddJobExecutionHistory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns batch job history information.
'		Array/Null

Const bjJobExecutionHistoryID				= 0
Const bjJobExecutionHistoryJobID			= 1
Const bjJobExecutionHistoryState			= 2
Const bjJobExecutionHistoryDateStarted		= 3
Const bjJobExecutionHistoryDateFinished		= 4
Const bjJobExecutionHistoryNote				= 5
Const bjJobExecutionHistoryErrorNumber		= 6
Const bjJobExecutionHistoryErrorSource		= 7
Const bjJobExecutionHistoryErrorDescription	= 8
Const bjJobExecutionHistoryDateCreated		= 9
Const bjJobExecutionHistoryDateModified		= 10

Function GetJobExecutionHistory(JobExecutionHistoryID)
	Dim returnValue, Cmd, ary(10)
	returnValue = Null
	If Not IsEmpty(JobExecutionHistoryID) and Not IsNull(JobExecutionHistoryID) and IsNumeric(JobExecutionHistoryID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_GetJobExecutionHistory")
		Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamInput,, JobExecutionHistoryID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@JobID", adInteger, adParamOutput)											'2
		Cmd.Parameters.Append Cmd.CreateParameter("@State", adBoolean, adParamOutput)											'3
		Cmd.Parameters.Append Cmd.CreateParameter("@DateStarted", adDBTimeStamp, adParamOutput)									'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateFinished", adDBTimeStamp, adParamOutput)								'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Note", adVarChar, adParamOutput, 500)										'6
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorNumber", adInteger, adParamOutput)										'7
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorSource", adVarChar, adParamOutput, 100)								'8
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorDescription", adVarChar, adParamOutput, 100)							'9
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)									'10
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)								'11
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			ary(bjJobExecutionHistoryID)				= JobExecutionHistoryID
			ary(bjJobExecutionHistoryJobID)				= Cmd.Parameters(2).Value
			ary(bjJobExecutionHistoryState)				= Cmd.Parameters(3).Value
			ary(bjJobExecutionHistoryDateStarted)		= Cmd.Parameters(4).Value
			ary(bjJobExecutionHistoryDateFinished)		= Cmd.Parameters(5).Value
			ary(bjJobExecutionHistoryNote)				= Cmd.Parameters(6).Value
			ary(bjJobExecutionHistoryErrorNumber)		= Cmd.Parameters(7).Value
			ary(bjJobExecutionHistoryErrorSource)		= Cmd.Parameters(8).Value
			ary(bjJobExecutionHistoryErrorDescription)	= Cmd.Parameters(9).Value
			ary(bjJobExecutionHistoryDateCreated)		= Cmd.Parameters(10).Value
			ary(bjJobExecutionHistoryDateModified)		= Cmd.Parameters(11).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetJobExecutionHistory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all batch jobs historyies.
'		Note: Constants defined above
'		Array/Null

Function GetJobExecutionHistories(JobID)
	Dim returnValue, Cmd, resultSet
	returnValue = Null
	If Not IsEmpty(JobID) and Not IsNull(JobID) and IsNumeric(JobID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_GetJobExecutionHistories")
		Cmd.Parameters.Append Cmd.CreateParameter("@JobID", adInteger, adParamInput,, JobID)	'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetJobExecutionHistories = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns batch job history information for the last run.
'		Note: Constants defined above
'		Array/Null

Function GetJobExecutionHistoryLast(JobID)
	Dim returnValue, Cmd, ary(10)
	returnValue = Null
	If Not IsEmpty(JobID) and Not IsNull(JobID) and IsNumeric(JobID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_GetJobExecutionHistoryLast")
		Cmd.Parameters.Append Cmd.CreateParameter("@JobID", adInteger, adParamInput,, JobID)									'1
		Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamOutput)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@State", adBoolean, adParamOutput)											'3
		Cmd.Parameters.Append Cmd.CreateParameter("@DateStarted", adDBTimeStamp, adParamOutput)									'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateFinished", adDBTimeStamp, adParamOutput)								'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Note", adVarChar, adParamOutput, 500)										'6
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorNumber", adInteger, adParamOutput)										'7
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorSource", adVarChar, adParamOutput, 100)								'8
		Cmd.Parameters.Append Cmd.CreateParameter("@ErrorDescription", adVarChar, adParamOutput, 100)							'9
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)									'10
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)								'11
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			ary(bjJobExecutionHistoryID)				= Cmd.Parameters(2).Value
			ary(bjJobExecutionHistoryJobID)				= JobID
			ary(bjJobExecutionHistoryState)				= Cmd.Parameters(3).Value
			ary(bjJobExecutionHistoryDateStarted)		= Cmd.Parameters(4).Value
			ary(bjJobExecutionHistoryDateFinished)		= Cmd.Parameters(5).Value
			ary(bjJobExecutionHistoryNote)				= Cmd.Parameters(6).Value
			ary(bjJobExecutionHistoryErrorNumber)		= Cmd.Parameters(7).Value
			ary(bjJobExecutionHistoryErrorSource)		= Cmd.Parameters(8).Value
			ary(bjJobExecutionHistoryErrorDescription)	= Cmd.Parameters(9).Value
			ary(bjJobExecutionHistoryDateCreated)		= Cmd.Parameters(10).Value
			ary(bjJobExecutionHistoryDateModified)		= Cmd.Parameters(11).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetJobExecutionHistoryLast = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates job execution history state.
'		Error True/False

Function SetJobExecutionHistoryState(JobExecutionHistoryID, State)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(JobExecutionHistoryID) and Not IsNull(JobExecutionHistoryID) and IsNumeric(JobExecutionHistoryID) Then
		If Not IsEmpty(State) and Not IsNull(State) and IsNumeric(State) Then
			State = CBool(State)
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_SetJobExecutionHistoryState")
			Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamInput,, JobExecutionHistoryID)
			Cmd.Parameters.Append Cmd.CreateParameter("@State", adBoolean, adParamInput,, State)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetJobExecutionHistoryState = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates job execution history start date.
'		Error True/False

Function SetJobExecutionHistoryDateStarted(JobExecutionHistoryID, DateStarted)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(JobExecutionHistoryID) and Not IsNull(JobExecutionHistoryID) and IsNumeric(JobExecutionHistoryID) Then
		If Not IsEmpty(DateStarted) and Not IsNull(DateStarted) and IsDate(DateStarted) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_SetJobExecutionHistoryDateStarted")
			Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamInput,, JobExecutionHistoryID)
			Cmd.Parameters.Append Cmd.CreateParameter("@DateStarted", adDBTimeStamp, adParamInput,, DateStarted)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetJobExecutionHistoryDateStarted = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates job execution history finish date.
'		Error True/False

Function SetJobExecutionHistoryDateFinished(JobExecutionHistoryID, DateFinished)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(JobExecutionHistoryID) and Not IsNull(JobExecutionHistoryID) and IsNumeric(JobExecutionHistoryID) Then
		If Not IsEmpty(DateFinished) and Not IsNull(DateFinished) and IsDate(DateFinished) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_SetJobExecutionHistoryDateFinished")
			Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamInput,, JobExecutionHistoryID)
			Cmd.Parameters.Append Cmd.CreateParameter("@DateFinished", adDBTimeStamp, adParamInput,, DateFinished)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetJobExecutionHistoryDateFinished = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates job execution history note.
'		Error True/False

Function SetJobExecutionHistoryNote(JobExecutionHistoryID, Note)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(JobExecutionHistoryID) and Not IsNull(JobExecutionHistoryID) and IsNumeric(JobExecutionHistoryID) Then
		If Not IsEmpty(Note) and Not IsNull(Note) and Len(Note) > 0 Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_SetJobExecutionHistoryNote")
			Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamInput,, JobExecutionHistoryID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Note", adVarChar, adParamInput, 500, Note)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetJobExecutionHistoryNote = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates job execution history error.
'		Error True/False

Function SetJobExecutionHistoryError(JobExecutionHistoryID, ErrorNumber, ErrorSource, ErrorDescription)
	Dim returnValue, Cmd
	returnValue = True
	If Not IsEmpty(JobExecutionHistoryID) and Not IsNull(JobExecutionHistoryID) and IsNumeric(JobExecutionHistoryID) Then
		If Not IsEmpty(ErrorNumber) and Not IsNull(ErrorNumber) and IsNumeric(ErrorNumber) and ErrorNumber <> 0 Then
			If Not Len(ErrorSource) > 0 Then ErrorSource = Null End If
			If Not Len(ErrorDescription) > 0 Then ErrorDescription = Null End If
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.BatchJob_SetJobExecutionHistoryError")
			Cmd.Parameters.Append Cmd.CreateParameter("@JobExecutionHistoryID", adInteger, adParamInput,, JobExecutionHistoryID)
			Cmd.Parameters.Append Cmd.CreateParameter("@ErrorNumber", adInteger, adParamInput,, ErrorNumber)
			Cmd.Parameters.Append Cmd.CreateParameter("@ErrorSource", adVarChar, adParamInput, 100, ErrorSource)
			Cmd.Parameters.Append Cmd.CreateParameter("@ErrorDescription", adVarChar, adParamInput, 100, ErrorDescription)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetJobExecutionHistoryError = returnValue
End Function
%>