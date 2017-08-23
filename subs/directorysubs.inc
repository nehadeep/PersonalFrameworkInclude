<%
	'	DESCRIPTION:
	'		Functions for directory information.
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc

	' Directory Database Cluster Property
	Dim ConnectionType_Directory
	ConnectionType_Directory = ctReadWrite


'***************************************************************
'	DESCRIPTION:
'		Adds a directory.
'		Returns DirectoryID/Null

Function AddDirectory(Name, Description, FolderRootName, FolderRootDescription)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(IsEmpty(FolderRootName)) and Not(IsNull(FolderRootName)) and Len(FolderRootName) > 0) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			If(Not(Len(FolderRootDescription) > 0)) Then FolderRootDescription = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_AddDirectory")
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)										'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderRootName", adVarChar, adParamInput, 75, FolderRootName)					'3
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderRootDescription", adVarChar, adParamInput, 500, FolderRootDescription)	'4
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamOutput)											'5
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(5).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddDirectory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a directory.
'		Error True/False

Function DeleteDirectory(DirectoryID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_DeleteDirectory")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteDirectory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a directory.
'		Error True/False

Function UpdateDirectory(DirectoryID, Name, Description)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_UpdateDirectory")
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
					returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateDirectory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a directory.
'		Array/Null

Const drDirectoryID				= 0
Const drDirectoryKey			= 1
Const drDirectoryName			= 2
Const drDirectoryDescription	= 3
Const drDirectoryDateCreated	= 4
Const drDirectoryDateModified	= 5

Function GetDirectory(DirectoryID)
	Dim returnValue, ary(5)
	returnValue = Null
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetDirectory")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryKey", adChar, adParamOutput, 32)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'6
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			ary(drDirectoryID)				= DirectoryID
			ary(drDirectoryKey)				= Cmd.Parameters(2).Value
			ary(drDirectoryName)			= Cmd.Parameters(3).Value
			ary(drDirectoryDescription)		= Cmd.Parameters(4).Value
			ary(drDirectoryDateCreated)		= Cmd.Parameters(5).Value
			ary(drDirectoryDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetDirectory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all directories.
'		Array/Null

'	Constants defined in GetDirectory

Function GetDirectories()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Directory)
	Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetDirectories")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Directory, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Directory)
	GetDirectories = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a DirectoryID from the guid.
'		Returns DirectoryID/Null

Function GetDirectoryID(DirectoryKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(DirectoryKey)) and Not(IsNull(DirectoryKey)) and Len(DirectoryKey) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetDirectoryID")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryKey", adChar, adParamInput, 32, DirectoryKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetDirectoryID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a directory name.
'		Returns DirectoryName/Null

Function GetDirectoryName(DirectoryID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetDirectoryName")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetDirectoryName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns folder list for the current directory (flat view).
'		Returns Array/Null

'	Constants defined in GetFolder

Function GetDirectoryFolders(DirectoryID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetDirectoryFolders")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Directory, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetDirectoryFolders = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a folder.
'		Returns FolderID/Null

Function AddFolder(DirectoryID, FolderParentID, Name, Description)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		If(Not(IsEmpty(FolderParentID)) and Not(IsNull(FolderParentID)) and IsNumeric(FolderParentID)) Then	
			If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
				If(Not(Len(Description) > 0)) Then Description = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_AddFolder")
				Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderParentID", adInteger, adParamInput,, FolderParentID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'3
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)						'5
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(5).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a folder.
'		Error True/False

Function DeleteFolder(FolderID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_DeleteFolder")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a folder.
'		Error True/False

Function UpdateFolder(FolderID, Name, Description)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_UpdateFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Moves one or more folders from one location to another.
'		Error True/False

Function MoveFolder(SourceFolderID, DestinationFolderID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(SourceFolderID)) and Not(IsNull(SourceFolderID)) and IsNumeric(SourceFolderID)) Then
		If(Not(IsEmpty(DestinationFolderID)) and Not(IsNull(DestinationFolderID)) and IsNumeric(DestinationFolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_MoveFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@SourceFolderID", adInteger, adParamInput,, SourceFolderID)
			Cmd.Parameters.Append Cmd.CreateParameter("@DestinationFolderID", adInteger, adParamInput,, DestinationFolderID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	MoveFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a folder.
'		Array/Null

Const drFolderID			= 0
Const drFolderKey			= 1
Const drFolderCode			= 2
Const drFolderDirectoryID	= 3
Const drFolderName			= 4
Const drFolderDescription	= 5
Const drFolderDateCreated	= 6
Const drFolderDateModified	= 7

Function GetFolder(FolderID)
	Dim returnValue, ary(7)
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolder")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamOutput, 15)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamOutput)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'8
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			ary(drFolderID)					= FolderID
			ary(drFolderKey)				= Cmd.Parameters(2).Value
			ary(drFolderCode)				= Cmd.Parameters(3).Value
			ary(drFolderDirectoryID)		= Cmd.Parameters(4).Value
			ary(drFolderName)				= Cmd.Parameters(5).Value
			ary(drFolderDescription)		= Cmd.Parameters(6).Value
			ary(drFolderDateCreated)		= Cmd.Parameters(7).Value
			ary(drFolderDateModified)		= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a folder code.
'		Error True/False

Function SetFolderCode(FolderID, FolderCode)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(FolderCode)) and Not(IsNull(FolderCode)) and Len(FolderCode) > 0) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_SetFolderCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamInput, 15, FolderCode)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetFolderCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a folders name.
'		Error True/False

Function SetFolderName(FolderID, FolderName)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(FolderName)) and Not(IsNull(FolderName)) and (Len(FolderName) > 0)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_SetFolderName")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, FolderName)			'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetFolderName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a FolderID from the guid.
'		Returns FolderID/Null

Function GetFolderID(FolderKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderKey)) and Not(IsNull(FolderKey)) and Len(FolderKey) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderID")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderKey", adChar, adParamInput, 32, FolderKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a FolderID from the FolderCode.
'		Returns FolderID/Null

Function GetFolderIDByCode(FolderCode)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderCode)) and Not(IsNull(FolderCode)) and Len(FolderCode) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamInput, 15, FolderCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a folder name.
'		Returns FolderName/Null

Function GetFolderName(FolderID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderName")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)			'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderName = returnValue
End Function


'***************************************************************
'DESCRIPTION:
'	Returns a FolderCode from a FolderID
'	Returns FolderCode/NULL

Function GetFolderCode(FolderID)
	Dim returnValue
	returnValue=Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd=CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamOutput,15)		'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a folders parent FolderID.
'		Returns FolderParentID/Null

Function GetFolderParentID(FolderID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderParentID")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderParentID", adInteger, adParamOutput)			'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderParentID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a folders parent folder.
'		Array/Null

'	Constants defined in GetFolder

Function GetFolderParent(FolderID)
	Dim returnValue, ary(7)
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderParent")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamOutput, 15)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamOutput)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'8
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			ary(drFolderID)					= FolderID
			ary(drFolderKey)				= Cmd.Parameters(2).Value
			ary(drFolderCode)				= Cmd.Parameters(3).Value
			ary(drFolderDirectoryID)		= Cmd.Parameters(4).Value
			ary(drFolderName)				= Cmd.Parameters(5).Value
			ary(drFolderDescription)		= Cmd.Parameters(6).Value
			ary(drFolderDateCreated)		= Cmd.Parameters(7).Value
			ary(drFolderDateModified)		= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderParent = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the folderID for the root folder in the directory.
'		Returns FolderID/Null

Function GetFolderDirectoryRootID(DirectoryID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderDirectoryRootID")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderDirectoryRootID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a directories root folder.
'		Array/Null

'	Constants defined in GetFolder

Function GetFolderDirectoryRoot(DirectoryID)
	Dim returnValue, ary(7)
	returnValue = Null
	If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderDirectoryRoot")
		Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderKey", adChar, adParamOutput, 32)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamOutput, 15)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'8
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			ary(drFolderID)					= Cmd.Parameters(2).Value
			ary(drFolderKey)				= Cmd.Parameters(3).Value
			ary(drFolderCode)				= Cmd.Parameters(4).Value
			ary(drFolderDirectoryID)		= DirectoryID
			ary(drFolderName)				= Cmd.Parameters(5).Value
			ary(drFolderDescription)		= Cmd.Parameters(6).Value
			ary(drFolderDateCreated)		= Cmd.Parameters(7).Value
			ary(drFolderDateModified)		= Cmd.Parameters(8).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderDirectoryRoot = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns folder list containing the children.
'		Array/Null

Const drFolderChildrenID			= 0
Const drFolderChildrenKey			= 1
Const drFolderChildrenCode			= 2
Const drFolderChildrenDirectoryID	= 3
Const drFolderChildrenSymbolic		= 4
Const drFolderChildrenName			= 5
Const drFolderChildrenDescription	= 6
Const drFolderChildrenDateCreated	= 7
Const drFolderChildrenDateModified	= 8

Function GetFolderChildren(FolderID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderChildren")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Directory, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetFolderChildren = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns folder list containing Children folder with active products (complete path to the root).
'		Returns Array/Null


Function GetFolderActiveChildren(FolderID, OriginalCatalogID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID) and Not(IsEmpty(OriginalCatalogID)) and Not(IsNull(OriginalCatalogID)) and IsNumeric(OriginalCatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadOnly)
		Set Cmd = CreateSQLProcedure(ctReadOnly, "WebDB.dbo.Directory_GetFolderActiveChildren")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)		'2
		Set resultSet = ExecuteSQLProcedure(ctReadOnly, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadOnly)
	End If
	GetFolderActiveChildren = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a folder location (complete path to the root).
'		Returns Array/Null

'	Constants defined in GetFolder

Function GetFolderLocation(FolderID)
	Dim returnValue, resultSet, ary, tempary, i
	returnValue = Null
	i = 0
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderLocation")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'1
		Set resultSet = CreateObject("ADODB.Recordset")
		resultSet.CursorType = adOpenForwardOnly
		resultSet.CursorLocation = adUseClient
		resultSet.LockType = adLockReadOnly
		resultSet.CacheSize = 100
		Set resultSet.Source = Cmd
		resultSet.Open
		While(Not(TypeName(resultSet) = "Nothing"))
			If(resultSet.State = adstateOpen) Then
				If Not resultSet.EOF Then
					tempary = resultSet.GetRows
					If(Not(IsArray(ary))) Then
						ary = tempary
					Else
						ReDim Preserve ary(Ubound(ary, 1), i)
						ary(drFolderID,i)			= tempary(drFolderID,0)
						ary(drFolderKey,i)			= tempary(drFolderKey,0)
						ary(drFolderCode,i)			= tempary(drFolderCode,0)
						ary(drFolderDirectoryID,i)	= tempary(drFolderDirectoryID,0)
						ary(drFolderName,i)			= tempary(drFolderName,0)
						ary(drFolderDescription,i)	= tempary(drFolderDescription,0)
						ary(drFolderDateCreated,i)	= tempary(drFolderDateCreated,0)
						ary(drFolderDateModified,i)	= tempary(drFolderDateModified,0)
					End If
				End If
			End If
			Set resultSet = resultSet.NextRecordset
			i = i + 1
		Wend
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
		returnValue = ary
	End If
	GetFolderLocation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a folder is a leaf of the directory tree.
'		Returns True/False

Function FolderIsLeaf(FolderID)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_FolderIsLeaf")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Leaf", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	FolderIsLeaf = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a folder is the root of the directory tree.
'		Returns True/False

Function FolderIsRoot(FolderID)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_FolderIsRoot")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Root", adBoolean, adParamOutput)
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	FolderIsRoot = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a folder link to another folder.
'		Note:	Only the system should create a non symbolic
'				hard link.
'		Returns FolderHardLinkID/Null

Function AddFolderHardLink(FolderID, FolderParentID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(FolderParentID)) and Not(IsNull(FolderParentID)) and IsNumeric(FolderParentID)) Then	
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_AddFolderHardLink")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderParentID", adInteger, adParamInput,, FolderParentID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Symbolic", adBoolean, adParamInput,, True)					'3
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderHardLinkID", adInteger, adParamOutput)				'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddFolderHardLink = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a folders link to another folder.
'		Note:	Only the system should delete a non symbolic
'				hard link.
'		Error True/False

Function DeleteFolderHardLink(FolderHardLinkID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(FolderHardLinkID)) and Not(IsNull(FolderHardLinkID)) and IsNumeric(FolderHardLinkID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_DeleteFolderHardLink")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderHardLinkID", adInteger, adParamInput,, FolderHardLinkID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Symbolic", adBoolean, adParamInput,, True)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteFolderHardLink = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a folder map to another folder.
'		Returns FolderMapID/Null

Function AddFolderMap(FolderID, MappedFolderID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(MappedFolderID)) and Not(IsNull(MappedFolderID)) and IsNumeric(MappedFolderID)) Then	
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_AddFolderMap")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@MappedFolderID", adInteger, adParamInput,, MappedFolderID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderMapID", adInteger, adParamOutput)						'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddFolderMap = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a folders map from another folder.
'		Error True/False

Function DeleteFolderMap(FolderMapID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(FolderMapID)) and Not(IsNull(FolderMapID)) and IsNumeric(FolderMapID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_DeleteFolderMap")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderMapID", adInteger, adParamInput,, FolderMapID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteFolderMap = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns folder list containing map folders.
'		Array/Null

Const drFolderMapID						= 0
Const drFolderMappedFolderID			= 1
Const drFolderMapFolderKey				= 2
Const drFolderMapFolderCode				= 3
Const drFolderMapFolderSortOptionID		= 4
Const drFolderMapFolderDirectoryID		= 5
Const drFolderMapFolderName				= 6
Const drFolderMapFolderDescription		= 7
Const drFolderMapMappedFolderID			= 8
Const drFolderMapMapFolderKey			= 9
Const drFolderMapMapFolderCode			= 10
Const drFolderMapMapFolderSortOptionID	= 11
Const drFolderMapMapFolderDirectoryID	= 12
Const drFolderMapMapFolderName			= 13
Const drFolderMapMapFolderDescription	= 14
Const drFolderMapDateCreated			= 15
Const drFolderMapDateModified			= 16

Function GetFolderMaps(FolderID, DirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Call OpenSQLConnection(ConnectionType_Directory)
			Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetFolderMaps")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Directory, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Directory)
		End If
	End If
	GetFolderMaps = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a folder has any products.
'		Returns True/False

Function FolderHasProduct(LogicalCatalogID, FolderID)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Call OpenSQLConnection(ConnectionType_Directory)
			Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_FolderHasProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Product", adBoolean, adParamOutput)
			Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Directory)
		End If
	End If
	FolderHasProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a folder has any active "non-disco" products.
'		Returns True/False

Function FolderHasActiveProduct(LogicalCatalogID, FolderID)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Call OpenSQLConnection(ConnectionType_Directory)
			Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_FolderHasActiveProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Product", adBoolean, adParamOutput)
			Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Directory)
		End If
	End If
	FolderHasActiveProduct = returnValue
End Function


'Crap Delete These...
'************************************************************************************************************************
Function GetDescendantFolderIDsCommaDelimitedList(FolderID, ReturnOnPGMFolderIDs)

	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetDescendantFolderIDsCommaDelimitedList")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)								'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ReturnOnlyPGMFolderIDs", adBoolean, adParamInput,, ReturnOnPGMFolderIDs)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CommaDelimitedList", adVarChar, adParamOutput, 5000)							'3
		Call ExecuteSQLProcedure(ConnectionType_Directory, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)
	End If
	GetDescendantFolderIDsCommaDelimitedList = returnValue

end function

'**************************************************************************************************************************
Const GetAllFolders_FolderID	= 0
Const GetAllFolders_Name		= 1
Const GetAllFolders_FolderCode	= 2

' Level=1 for Categories, Level=2 for Classes, Level=3 for PGMs
Function GetAllFolders(Level)
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Directory)
		Set Cmd = CreateSQLProcedure(ConnectionType_Directory, "WebDB.dbo.Directory_GetAllFoldersForLevel")
		Cmd.Parameters.Append Cmd.CreateParameter("@Level", adInteger, adParamInput,, Level)			'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Directory, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Directory)

	GetAllFolders = returnValue
end function

%>