<%
	'	DESCRIPTION:
	'		Functions for attribute information.
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc

	' Attribute Database Cluster Property
	Dim ConnectionType_Attribute
	ConnectionType_Attribute 					= ctReadWrite

	'Attribute Constants
	Const aeNameKey								= "8028B55B588347A6B68C52AB1D905910"
	Const aeObjectiveKey						= "7AB86FE15960481E898F8FA33C495457"
	Const aeSubjectiveKey						= "A4C3BF6C9C244DB38D3852632866B8ED"
	Const aeBodyKey								= "40FFC36C16804BC1BEE40DCAE0F10702"
	Const aePhotoFileKey						= "C13520FCDA7F4E16BDA85E6D1E7D4A9D"
	Const aePhotoCaptionKey 					= "1541A8B8CD2B42F1A7DD111D419141EF"
	Const aePhotoSpinSetKey						= "B992ED632AC4455C8D6A1EDB49EEB26C"

	'Report Constants
	Const aeTotalManufacturersReport			= "01F65FED66E140D48C3581287A323394"
	Const aeTotalProductsReport					= "104F14DC45BA4D36971CC00AA291828C"

	Const aeCDWTotalProductAttributesReport		= "1D5D93ACE98A49AEAA530167C6B76E14"
	Const aeCDWTotalProductContentReport		= "182D6F4C59184651A2F56A0EAAA24153"
	Const aeCDWTotalAttributesReport			= "84C1FC641F0749F4A841AEA72C53282D"
	Const aeCDWTotalContentReport				= "55D266B1025545659505EEE0251A27EC"
	Const aeCDWTotalPhotoCaptionsReport			= "91661CBACC524923A663AE4F1BC4F0F0"
	Const aeCDWAverageAttributesReport			= "CE8DCBA0A71E4226BDC6C5EC9EF12801"
	Const aeCDWAverageContentReport				= "86753D9E18A74D04AB1B5D5BCF8DC80B"
	Const aeCDWTotalAverageReport				= "5283573836C84F229911237E43F30A95"
	Const aeCDWContentAuthorsReport				= "E2509061E6224C4C898E52B69B70382C"
	Const aeCDWAttributeAuthorsReport			= "70FC950330824BE89DDE2160075076A3"

	Const aeCNetTotalProductAttributesReport	= "18D863CB4F084723B7D1F7C6E6B6F115"
	Const aeCNetTotalProductContentReport		= "2EC43BDD4742457DB3A4CA016D1198D7"
	Const aeCNetTotalAttributesReport			= "8EBC1C440A4E4CA7803B5693C5F779DF"
	Const aeCNetTotalContentReport				= "85119A26E1C0466A9AB837B6285945D2"
	Const aeCNetTotalPhotoCaptionsReport		= "76D685D096694B718923E77DDAE0A36A"
	Const aeCNetAverageAttributesReport			= "4D5E641287B34A55B5553D8E7178C2F7"
	Const aeCNetAverageContentReport			= "B352F800BFEC48C6A582C7AA0D058247"
	Const aeCNetTotalAverageReport				= "B8C719C681C847C99BB3D91B8F1B4272"
	Const aeCNetContentAuthorsReport			= "D924F5F6139A49D28CDC215DADDE4B34"
	Const aeCNetAttributeAuthorsReport			= "9C1A08D6E514445A90387C4942DFDE75"

	Const aeCNetWaitingReport					= "C2CB9ED013154F2B974DABA877533DB1"
	Const aeCNetFilledReport					= "149E805C8FA94A49BA5A0AD59190C06D"
	Const aeCNetNotRequestedReport				= "3A86108EFBE3428583D51D796A547FF6"

	Const aeRejectedProductsReport				= "9F6F0F53FCE84F47ACDB1ECA002CF35F"
	Const aeInProgressProductsReport			= "FA555808BC1F4580B8CF9E1FBA9BA24E"
	Const aeCompleteProductsReport				= "CCD6DDFDFE5543969912FAADEB7CC399"
	Const aeApprovedProductsReport				= "FBC8BC6E00C64A0180E1006F07DDA496"
	Const aeNewProductsReport					= "A64DB8CF56EE42C58ADBB3C40113A400"

	'Attribute Type Constants
	Const aeAttributeTypeMultiSelectValue		= "ADDFA8580FD84857A21E8CEC0D79C32A"
	Const aeAttributeTypeSingleSelectValue		= "B31644DC3D7D43CCA2FE93FA8A9A4613"

	'Attribute Status Constants
	Const aeStatusInProgress					= "71C720556E3448CBABC5ED96481618AC" ' New attribute that has not yet been submitted.
	Const aeStatusComplete						= "9F2594AF240D484EB09D9F4D2268B8E3" ' Submitted attribute to the powers that be for approval.
	Const aeStatusRejected						= "E2CABC10B3FA4AAA9C7474FB48693D82" ' Rejected attribute by the powers that be.
	Const aeStatusApproved						= "1C98D93F5F2D4171ADA78D804D402FE2" ' Approved attribute, and live.

'****************************************************************************************************************************
'*		Section Description:																								*
'*		Collections																											*
'*			Wrappers for collections 																						*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds a collection.
'		Returns CollectionID/Null

Function AddCollection(Name, Description)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddCollection")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddCollection = returnValue
End Function




'***************************************************************
'	DESCRIPTION:
'		Deletes a collection.
'		Error True/False

Function DeleteCollection(CollectionID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteCollection")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)
		Cmd.Parameters.Append Cmd.CreateParameter("@PgmID", adInteger, adParamInput,, AttributeID)	
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCollection = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a collection.
'		Error True/False

Function UpdateCollection(CollectionID, Name, Description)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateCollection")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateCollection = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an Collection.
'		Array/Null

Const aeCollectionID			= 0
Const aeCollectionKey			= 1
Const aeCollectionName			= 2
Const aeCollectionDescription	= 3
Const aeCollectionDateCreated	= 4
Const aeCollectionDateModified	= 5

Function GetCollection(CollectionID)
	Dim returnValue, ary(5), Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetCollection")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamOutput, 32)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'6
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeCollectionID)				= CollectionID
			ary(aeCollectionKey)			= Cmd.Parameters(2).Value
			ary(aeCollectionName)			= Cmd.Parameters(3).Value
			ary(aeCollectionDescription)	= Cmd.Parameters(4).Value
			ary(aeCollectionDateCreated)	= Cmd.Parameters(5).Value
			ary(aeCollectionDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetCollection = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a CollectionID from the guid.
'		Returns CollectionID/Null

Function GetCollectionID(CollectionKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionKey)) and Not(IsNull(CollectionKey)) and Len(CollectionKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetCollectionID")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamInput, 32, CollectionKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetCollectionID = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Attributes																											*
'*			Wrappers for attributes 																						*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an attribute.
'		Returns AttributeID/Null

Function AddAttribute(AttributeCode, Name, Description, AttributeTypeID, CollectionID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(IsEmpty(AttributeTypeID)) and Not(IsNull(AttributeTypeID)) and IsNumeric(AttributeTypeID)) Then
			If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
				If(Not(Len(AttributeCode) > 0)) Then AttributeCode = Null End If
				If(Not(Len(Description) > 0)) Then Description = Null End If
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttribute")
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamInput, 15, AttributeCode)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamInput,, AttributeTypeID)	'4
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'5
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)							'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(6).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddAttribute = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a attributes user information.
'		Error True/False

Function SetAttributeUserCreated(AttributeID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeUserCreated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a attributes user information.
'		Error True/False

Function SetAttributeUserModified(AttributeID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeUserModified")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeUserModified = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an attribute.
'		Error True/False

Function DeleteAttribute(AttributeID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttribute")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttribute = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates an attribute.
'		Error True/False

Function UpdateAttribute(AttributeID, Name, Description, AttributeTypeID, CollectionID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(IsEmpty(AttributeTypeID)) and Not(IsNull(AttributeTypeID)) and IsNumeric(AttributeTypeID)) Then
				If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
					If(Not(Len(Description) > 0)) Then Description = Null End If
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateAttribute")
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)			'1
					Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
					Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamInput,, AttributeTypeID)	'4
					Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'5
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If(Cmd.Parameters(0).Value = 0) Then
							returnValue = False
						End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	UpdateAttribute = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an attribute.
'		Array/Null

Const aeAttributeID							= 0
Const aeAttributeKey						= 1
Const aeAttributeCode						= 2
Const aeAttributeName						= 3
Const aeAttributeDescription				= 4
Const aeAttributeAttributeTypeID			= 5
Const aeAttributeAttributeTypeKey			= 6
Const aeAttributeAttributeTypeName			= 7
Const aeAttributeAttributeTypeDescription	= 8
Const aeAttributeCollectionID				= 9
Const aeAttributeCollectionKey				= 10
Const aeAttributeCollectionName				= 11
Const aeAttributeCollectionDescription		= 12
Const aeAttributeUserModified				= 13
Const aeAttributeUserCreated				= 14
Const aeAttributeDateCreated				= 15
Const aeAttributeDateModified				= 16

Function GetAttribute(AttributeID)
	Dim returnValue, ary(16), Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttribute")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamOutput, 15)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamOutput)					'6
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeKey", adChar, adParamOutput, 32)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeName", adVarChar, adParamOutput, 75)			'8
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeDescription", adVarChar, adParamOutput, 500)	'9
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)					'10
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamOutput, 32)					'11
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionName", adVarChar, adParamOutput, 75)				'12
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionDescription", adVarChar, adParamOutput, 500)		'13
		Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)				'14
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)					'15
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)					'16
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)				'17
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeAttributeID)						= AttributeID
			ary(aeAttributeKey)						= Cmd.Parameters(2).Value
			ary(aeAttributeCode)					= Cmd.Parameters(3).Value
			ary(aeAttributeName)					= Cmd.Parameters(4).Value
			ary(aeAttributeDescription)				= Cmd.Parameters(5).Value
			ary(aeAttributeAttributeTypeID)			= Cmd.Parameters(6).Value
			ary(aeAttributeAttributeTypeKey)		= Cmd.Parameters(7).Value
			ary(aeAttributeAttributeTypeName)		= Cmd.Parameters(8).Value
			ary(aeAttributeAttributeTypeDescription)= Cmd.Parameters(9).Value
			ary(aeAttributeCollectionID)			= Cmd.Parameters(10).Value
			ary(aeAttributeCollectionKey)			= Cmd.Parameters(11).Value
			ary(aeAttributeCollectionName)			= Cmd.Parameters(12).Value
			ary(aeAttributeCollectionDescription)	= Cmd.Parameters(13).Value
			ary(aeAttributeUserModified)			= Cmd.Parameters(14).Value
			ary(aeAttributeUserCreated)				= Cmd.Parameters(15).Value
			ary(aeAttributeDateCreated)				= Cmd.Parameters(16).Value
			ary(aeAttributeDateModified)			= Cmd.Parameters(17).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttribute = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a AttributeID from the guid.
'		Returns AttributeID/Null

Function GetAttributeID(AttributeKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeKey)) and Not(IsNull(AttributeKey)) and Len(AttributeKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeKey", adChar, adParamInput, 32, AttributeKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a AttributeID from the AttributeCode.
'		Returns AttributeID/Null

Function GetAttributeIDByCode(AttributeCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeCode)) and Not(IsNull(AttributeCode)) and Len(AttributeCode) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamInput, 15, AttributeCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an attribute name.
'		Returns AttributeName/Null

Function GetAttributeName(AttributeID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeName")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a attribute type.
'		Returns AttributeTypeID/Null

Function AddAttributeType(Name, Description)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeType")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddAttributeType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a attribute type.
'		Error True/False

Function DeleteAttributeType(AttributeTypeID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeTypeID)) and Not(IsNull(AttributeTypeID)) and IsNumeric(AttributeTypeID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeType")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamInput,, AttributeTypeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a attribute type.
'		Error True/False

Function UpdateAttributeType(AttributeTypeID, Name, Description)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateAttributeType")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamInput,, AttributeTypeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateAttributeType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an attribute type.
'		Array/Null

Const aeAttributeTypeID				= 0
Const aeAttributeTypeKey			= 1
Const aeAttributeTypeName			= 2
Const aeAttributeTypeDescription	= 3
Const aeAttributeTypeDateCreated	= 4
Const aeAttributeTypeDateModified	= 5

Function GetAttributeType(AttributeTypeID)
	Dim returnValue, ary(5), Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeTypeID)) and Not(IsNull(AttributeTypeID)) and IsNumeric(AttributeTypeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeType")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamInput,, AttributeTypeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)					'6
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeAttributeTypeID)				= AttributeTypeID
			ary(aeAttributeTypeKey)				= Cmd.Parameters(2).Value
			ary(aeAttributeTypeName)			= Cmd.Parameters(3).Value
			ary(aeAttributeTypeDescription)		= Cmd.Parameters(4).Value
			ary(aeAttributeTypeDateCreated)		= Cmd.Parameters(5).Value
			ary(aeAttributeTypeDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeType = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all attributeTypes.
'		Array/Null

' Constants defined above...

Function GetAttributeTypes()
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeTypes")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If not resultSet.EOF Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetAttributeTypes = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an AttributeTypeID from the guid.
'		Returns AttributeTypeID/Null

Function GetAttributeTypeID(AttributeTypeKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeTypeKey)) and Not(IsNull(AttributeTypeKey)) and Len(AttributeTypeKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeKey", adChar, adParamInput, 32, AttributeTypeKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeTypeID = returnValue
End Function


'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds a attribute value.
'		Returns AttributeValueID/Null

Function AddAttributeValue(ValueCode, AttributeID, Value, UnitID, Active)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0) Then
			If(Not(Len(ValueCode) > 0)) Then ValueCode = Null End If
			If(Not(Len(UnitID) > 0)) Then UnitID = Null End If
			If(Not(Len(Active) > 0)) Then Active = True End If
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeValue")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueCode", adVarChar, adParamInput, 15, ValueCode)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamInput, 4000, Value)						'3
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)							'4
			Cmd.Parameters.Append Cmd.CreateParameter("@Active", adBoolean, adParamInput,, Active)							'5
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamOutput)						'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(6).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddAttributeValue = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a attributevalues user information.
'		Error True/False

Function SetAttributeValueUserCreated(AttributeValueID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeValueUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeValueUserCreated = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a AttributeValues user information.
'		Error True/False

Function SetAttributeValueUserModified(AttributeValueID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeValueUserModified")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeValueUserModified = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates an attribute value.
'		Error True/False

Function UpdateAttributeValue(AttributeValueID, Value, UnitID, Active)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeValueID)) and Not(IsNull(AttributeValueID)) and Len(AttributeValueID) > 0) Then
		If(Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateAttributeValue")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamInput, 4000, Value)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)						'3
			Cmd.Parameters.Append Cmd.CreateParameter("@Active", adBoolean, adParamInput,, Active)						'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateAttributeValue = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a attribute value.
'		Error True/False

Function DeleteAttributeValue(AttributeValueID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeValueID)) and Not(IsNull(AttributeValueID)) and IsNumeric(AttributeValueID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeValue")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeValue = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes all an attributes values.
'		Error True/False

Function DeleteAttributeValues(AttributeID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeValues")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeValues = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an attribute value.
'		Array/Null

Const aeAttributeValueID							= 0
Const aeAttributeValueKey							= 1
Const aeAttributeValueCode							= 2
Const aeAttributeValueAttributeID					= 3
Const aeAttributeValueAttributeKey					= 4
Const aeAttributeValueAttributeCode					= 5
Const aeAttributeValueAttributeName					= 6
Const aeAttributeValueAttributeDescription			= 7
Const aeAttributeValueAttributeTypeID				= 8
Const aeAttributeValueAttributeTypeKey				= 9
Const aeAttributeValueAttributeTypeName				= 10
Const aeAttributeValueAttributeTypeDescription		= 11
Const aeAttributeValueCollectionID					= 12
Const aeAttributeValueCollectionKey					= 13
Const aeAttributeValueCollectionName				= 14
Const aeAttributeValueCollectionDescription			= 15
Const aeAttributeValueValue							= 16
Const aeAttributeValueSearchableValue				= 17
Const aeAttributeValueNumericValue					= 18
Const aeAttributeValueUnitID						= 19
Const aeAttributeValueUnitKey						= 20
Const aeAttributeValueUnitCode						= 21
Const aeAttributeValueUnitName						= 22
Const aeAttributeValueUnitDescription				= 23
Const aeAttributeValueUnitCollection				= 24
Const aeAttributeValueActive						= 25
Const aeAttributeValueUserModified					= 26
Const aeAttributeValueUserCreated					= 27
Const aeAttributeValueDateCreated					= 28
Const aeAttributeValueDateModified					= 29

Function GetAttributeValue(AttributeValueID)
	Dim returnValue, ary(29), Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeValueID)) and Not(IsNull(AttributeValueID)) and IsNumeric(AttributeValueID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValue")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueKey", adChar, adParamOutput, 32)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueCode", adVarChar, adParamOutput, 15)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamOutput)								'4
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeKey", adChar, adParamOutput, 32)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeCode", adVarChar, adParamOutput, 15)						'6
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeName", adVarChar, adParamOutput, 75)						'7
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeDescription", adVarChar, adParamOutput, 500)				'8
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeID", adInteger, adParamOutput)							'9
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeKey", adChar, adParamOutput, 32)						'10
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeName", adVarChar, adParamOutput, 75)					'11
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeTypeDescription", adVarChar, adParamOutput, 500)			'12
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)							'13
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamOutput, 32)							'14
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionName", adVarChar, adParamOutput, 75)						'15
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionDescription", adVarChar, adParamOutput, 500)				'16
		Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput, 4000)								'17
		Cmd.Parameters.Append Cmd.CreateParameter("@SearchableValue", adVarChar, adParamOutput, 100)					'18
		Cmd.Parameters.Append Cmd.CreateParameter("@NumericValue", adDouble, adParamOutput)								'19
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamOutput)									'20
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitKey", adChar, adParamOutput, 32)								'21
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitCode", adVarChar, adParamOutput, 15)							'22
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitName", adVarChar, adParamOutput, 75)							'23
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitDescription", adVarChar, adParamOutput, 500)					'24
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitCollectionID", adInteger, adParamOutput)						'25
		Cmd.Parameters.Append Cmd.CreateParameter("@Active", adBoolean, adParamOutput)									'26
		Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)						'27
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)							'28
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)							'29
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)						'30
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeAttributeValueID)							= AttributeValueID
			ary(aeAttributeValueKey)						= Cmd.Parameters(2).Value
			ary(aeAttributeValueCode)						= Cmd.Parameters(3).Value
			ary(aeAttributeValueAttributeID)				= Cmd.Parameters(4).Value
			ary(aeAttributeValueAttributeKey)				= Cmd.Parameters(5).Value
			ary(aeAttributeValueAttributeCode)				= Cmd.Parameters(6).Value
			ary(aeAttributeValueAttributeName)				= Cmd.Parameters(7).Value
			ary(aeAttributeValueAttributeDescription)		= Cmd.Parameters(8).Value
			ary(aeAttributeValueAttributeTypeID)			= Cmd.Parameters(9).Value
			ary(aeAttributeValueAttributeTypeKey)			= Cmd.Parameters(10).Value
			ary(aeAttributeValueAttributeTypeName)			= Cmd.Parameters(11).Value
			ary(aeAttributeValueAttributeTypeDescription)	= Cmd.Parameters(12).Value
			ary(aeAttributeValueCollectionID)				= Cmd.Parameters(13).Value
			ary(aeAttributeValueCollectionKey)				= Cmd.Parameters(14).Value
			ary(aeAttributeValueCollectionName)				= Cmd.Parameters(15).Value
			ary(aeAttributeValueCollectionDescription)		= Cmd.Parameters(16).Value
			ary(aeAttributeValueValue)						= Cmd.Parameters(17).Value
			ary(aeAttributeValueSearchableValue)			= Cmd.Parameters(18).Value
			ary(aeAttributeValueNumericValue)				= Cmd.Parameters(19).Value
			ary(aeAttributeValueUnitID)						= Cmd.Parameters(20).Value
			ary(aeAttributeValueUnitKey)					= Cmd.Parameters(21).Value
			ary(aeAttributeValueUnitCode)					= Cmd.Parameters(22).Value
			ary(aeAttributeValueUnitName)					= Cmd.Parameters(23).Value
			ary(aeAttributeValueUnitDescription)			= Cmd.Parameters(24).Value
			ary(aeAttributeValueUnitCollection)				= Cmd.Parameters(25).Value
			ary(aeAttributeValueActive)						= Cmd.Parameters(26).Value
			ary(aeAttributeValueUserModified)				= Cmd.Parameters(27).Value
			ary(aeAttributeValueUserCreated)				= Cmd.Parameters(28).Value
			ary(aeAttributeValueDateCreated)				= Cmd.Parameters(29).Value
			ary(aeAttributeValueDateModified)				= Cmd.Parameters(30).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeValue = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all values for the current attribute.
'		Array/Null

' Constants defined above...

Function GetAttributeValues(AttributeID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValues")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeValues = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all values for the current attribute.
'		Array/Null

' Constants defined above...

Function GetAttributeValuesbyPGM(AttributeID, ProductGroupList)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(ProductGroupList)) and Not(IsNull(ProductGroupList)) and Len(ProductGroupList) > 0 ) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValuesGroupFilter")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductGroupList", adVarChar, adParamInput,1000, ProductGroupList)	'1
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetAttributeValuesbyPGM = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a AttributeValueID from the guid.
'		Returns AttributeValueID/Null

Function GetAttributeValueID(AttributeValueKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeValueKey)) and Not(IsNull(AttributeValueKey)) and Len(AttributeValueKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValueID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueKey", adChar, adParamInput, 32, AttributeValueKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeValueID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a AttributeValueID from the AttributeValueCode.
'		Returns AttributeValueID/Null

Function GetAttributeValueIDByCode(AttributeValueCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeValueCode)) and Not(IsNull(AttributeValueCode)) and Len(AttributeValueCode) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValueIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueCode", adVarChar, adParamInput, 15, AttributeValueCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeValueIDByCode = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Product Versions																									*
'*			Wrappers for Product Versions																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds Product Version.
'		Returns VersionID/Null

Function AddProductVersion(ProductID, Action, UserName)
	Dim returnValue, Cmd
	returnValue = Null
	If (Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If (Not(IsEmpty(Action)) and Not(IsNull(Action)) and Len(Action) > 0 )Then
			If (Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 )Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddProductVersion")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Action", adVarChar, adParamInput,100, Action)			'2
				Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput,50, UserName)		'3
				Cmd.Parameters.Append Cmd.CreateParameter("@VersionID", adInteger, adParamOutput)					'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddProductVersion = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns product versions for the current product.
'		Array/Null

Const aeProductVersionVersionID				= 0
Const aeProductVersionVersionNumber			= 1
Const aeProductVersionProductID				= 2
Const aeProductVersionAction				= 3
Const aeProductVersionUserModified			= 4
Const aeProductVersionUserCreated			= 5
Const aeProductVersionDateCreated			= 6
Const aeProductVersionDateModified			= 7

Function GetProductVersions(ProductID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductVersions")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductVersions = returnValue
End Function
'****************************************************************************************************************************
'	DESCRIPTION:
'		Copies all of the attribute value assignments from one product to another.
'		Error True/False

Function CopyAttributeProductAssignments(OldProductID, NewProductID, AttributeStatusID, UserName)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(OldProductID)) and Not(IsNull(OldProductID)) and IsNumeric(OldProductID)) Then
		If(Not(IsEmpty(NewProductID)) and Not(IsNull(NewProductID)) and IsNumeric(NewProductID)) Then
			If(Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
				If(Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 ) Then
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_CopyAttributeValueProductAssignments")
					Cmd.Parameters.Append Cmd.CreateParameter("@OldProductID", adInteger, adParamInput,, OldProductID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@NewProductID", adInteger, adParamInput,, NewProductID)				'2
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'3
					Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput,50, UserName)					'4
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If(Cmd.Parameters(0).Value = 0) Then
							returnValue = False
						End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	CopyAttributeProductAssignments = returnValue
End Function
'****************************************************************************************************************************
'	DESCRIPTION:
'		Copies all of the content of one product to another.
'		Error True/False

Function CopyProductContent(OldProductID, NewProductID, AttributeStatusID, CollectionID, Username)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(OldProductID)) and Not(IsNull(OldProductID)) and IsNumeric(OldProductID)) Then
		If(Not(IsEmpty(NewProductID)) and Not(IsNull(NewProductID)) and IsNumeric(NewProductID)) Then
			If(Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
				If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
					If(Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0) Then
						Call OpenSQLConnection(ctReadWrite)
						Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_CopyProductContent")
						Cmd.Parameters.Append Cmd.CreateParameter("@OldProductID", adInteger, adParamInput,, OldProductID)				'1
						Cmd.Parameters.Append Cmd.CreateParameter("@NewProductID", adInteger, adParamInput,, NewProductID)				'2
						Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'3
						Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)				'4
						Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput,50, UserName)					'5
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
							If(Cmd.Parameters(0).Value = 0) Then
								returnValue = False
							End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	CopyProductContent = returnValue
End Function
'****************************************************************************************************************************
'	DESCRIPTION:
'		Copies all of the accessories of one product to another.
'		Error True/False

Function CopyProductAccessories(OldProductID, NewProductID, UserCreated)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(OldProductID)) and Not(IsNull(OldProductID)) and IsNumeric(OldProductID)) Then
		If(Not(IsEmpty(NewProductID)) and Not(IsNull(NewProductID)) and IsNumeric(NewProductID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_CopyProductAccessories")
			Cmd.Parameters.Append Cmd.CreateParameter("@OldProductID", adInteger, adParamInput,, OldProductID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@NewProductID", adInteger, adParamInput,, NewProductID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamInput, 50, UserCreated)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = False
				End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	CopyProductAccessories = returnValue
End Function
'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns all attribute value product assignments for the current version.
'		Array/Null
'		avpah = Attribute Value Product Assignment History

Const avpahAttributeValueProductAssignmentHistoryID	= 0
Const avpahAttributeValueProductAssignmentID		= 1
Const avpahAttributeID								= 2
Const avpahAttributeKey								= 3
Const avpahAttributeCode							= 4
Const avpahAttributeName							= 5
Const avpahDescription								= 6
Const avpahAttributeTypeID							= 7
Const avpahAttributeTypeKey							= 8
Const avpahAttributeTypeName						= 9
Const avpahAttributeTypeDescription					= 10
Const avpahCollectionID								= 11
Const avpahCollectionKey							= 12
Const avpahCollectionName							= 13
Const avpahCollectionDescription					= 14
Const avpahProductID								= 15
Const avpahAttributeValueID							= 16
Const avpahAttributeValueKey						= 17
Const avpahAttributeValueCode						= 18
Const avpahAttributeValueValue						= 19
Const avpahAttributeValueSearchableValue			= 20
Const avpahAttributeValueNumericValue				= 21
Const avpahAttributeValueUnitID						= 22
Const avpahUnitKey									= 23
Const avpahUnitCode									= 24
Const avpahUnitName									= 25
Const avpahUnitDescription							= 26
Const avpahUnitCollectionID							= 27
Const avpahAttributeValueActive						= 28
Const avpahAttributeStatusID						= 29
Const avpahAttributeStatusKey						= 30
Const avpahAttributeStatusName						= 31
Const avpahAttributeStatusDescription				= 32
Const avpahFolderID									= 33
Const avpahFolderKey								= 34
Const avpahFolderCode								= 35
Const avpahFolderSortOptionID						= 36
Const avpahFolderDirectoryID						= 37
Const avpahFolderName								= 38
Const avpahFolderDescription						= 39
Const avpahUserCreated								= 40
Const avpahUserModified								= 41
Const avpahDateCreated								= 42
Const avpahDateModified								= 43

Function GetAttributeValueProductAssignmentHistory(ProductID, CollectionID, DirectoryID, VersionID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
				If(Not(IsEmpty(VersionID)) and Not(IsNull(VersionID)) and IsNumeric(VersionID)) Then
					Call OpenSQLConnection(ConnectionType_Attribute)
					Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValueProductAssignmentHistory")
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
					Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'2
					Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'3
					Cmd.Parameters.Append Cmd.CreateParameter("@VersionID", adInteger, adParamInput,, VersionID)		'4
					Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
					If(resultSet.State = adstateOpen) Then
						If(Not(resultSet.EOF)) Then
							returnValue = resultSet.GetRows
						End If
						resultSet.Close
					End If
					Set resultSet = Nothing
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Attribute)
				End If
			End If
		End If
	End If
	GetAttributeValueProductAssignmentHistory = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns all attribute value product assignments for the current version.
'		Array/Null
'		pch = Product Content History

	Const pchProductContentHistoryID	= 0
	Const pchProductContentID			= 1
	Const pchContentTypeID				= 2
	Const pchContentTypeName			= 3
	Const pchContentTypeDescription		= 4
	Const pchValue						= 5
	Const pchCollectionID				= 6
	Const pchCollectionKey				= 7
	Const pchCollectionName				= 8
	Const pchCollectionDescription		= 9
	Const pchAttributeStatusID			= 10
	Const pchAttributeStatusKey			= 11
	Const pchAttributeStatusName		= 12
	Const pchAttributeStatusDescription	= 13
	Const pchSearchableValue			= 14
	Const pchUserModified				= 15
	Const pchUserCreated				= 16
	Const pchDateCreated				= 17
	Const pchDateModified				= 18

Function GetProductContentHistory(ProductID, Version)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(Version)) and Not(IsNull(Version)) and IsNumeric(Version)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentHistory")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Version", adInteger, adParamInput,, Version)	'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetProductContentHistory = returnValue
End Function
'******************************************************************************************************************************************
' Copies the archive value with the given version ProductID and VersionID. 

Function RollbackProductVersion(ProductID, VersionID)
	Dim returnValue
	returnValue = true
	If not isNull(ProductID) and not isEmpty(ProductID) and IsNumeric(ProductID) Then
		If not isNull(VersionID) and not isEmpty(VersionID) and IsNumeric(VersionID) Then
				Dim Cmd
				call OpenSQLConnection(ctReadWrite)
				set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_RollbackProductVersion")
				Cmd.CommandTimeOut = 1800
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
				Cmd.Parameters.Append Cmd.CreateParameter("@VersionID", adInteger, adParamInput,, VersionID)
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 Then
					returnValue = false
				End If
				set Cmd = Nothing
				call CloseSQLConnection(ctReadWrite)
			End If
	End If
	RollbackProductVersion = returnValue
End Function

'****************************************************************************************************************************
'*		Section Description:																								*
'*		AttributeStatuses																									*
'*			Wrappers for AttributeStatuses	 																				*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an attribute Status.
'		Returns AttributeStatusID/Null

Function AddAttributeStatus(Name, Description)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeStatus")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddAttributeStatus = returnValue
End Function


'****************************************************************************************************************************
'	DESCRIPTION:
'		Deletes an attributeStatus.
'		Error True/False

Function DeleteAttributeStatus(AttributeStatusID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeStatus")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeStatus = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates an attribute.
'		Error True/False

Function UpdateAttributeStatus(AttributeStatusID, Name, Description)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateAttributeStatus")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)							'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateAttributeStatus = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an attributeStatus.
'		Array/Null

Const aeAttributeStatusID						= 0
Const aeAttributeStatusKey						= 1
Const aeAttributeStatusName						= 2
Const aeAttributeStatusDescription				= 3
Const aeAttributeStatusDateCreated				= 4
Const aeAttributeStatusDateModified				= 5

Function GetAttributeStatus(AttributeStatusID)
	Dim returnValue, ary(5), Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttribute")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusKey", adChar, adParamOutput, 32)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)									'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)							'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)								'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)							'6
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeAttributeID)						= AttributeID
			ary(aeAttributeKey)						= Cmd.Parameters(2).Value
			ary(aeAttributeName)					= Cmd.Parameters(4).Value
			ary(aeAttributeDescription)				= Cmd.Parameters(5).Value
			ary(aeAttributeDateCreated)				= Cmd.Parameters(5).Value
			ary(aeAttributeDateModified)			= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeStatus = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a AttributeStatusID from the guid.
'		Returns AttributeID/Null

Function GetAttributeStatusID(AttributeStatusKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeStatusKey)) and Not(IsNull(AttributeStatusKey)) and Len(AttributeStatusKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeStatusID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusKey", adChar, adParamInput, 32, AttributeStatusKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeStatusID = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Attribute Folder Assignments																						*
'*			Wrappers for attribute folders																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an attribute to a folder.
'		Returns AttributeFolderAssignmentID/Null

Function AddAttributeFolderAssignment(AttributeID, FolderID, Required, Comparable, Searchable)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			If(Not(Len(Required) > 0)) Then Required = False End If
			If(Not(Len(Comparable) > 0)) Then Comparable = False End If
			If(Not(Len(Searchable) > 0)) Then Searchable = False End If
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeFolderAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Required", adBoolean, adParamInput,, Required)				'3
			Cmd.Parameters.Append Cmd.CreateParameter("@Comparable", adBoolean, adParamInput,, Comparable)			'4
			Cmd.Parameters.Append Cmd.CreateParameter("@Searchable", adBoolean, adParamInput,, Searchable)			'5
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderAssignmentID", adInteger, adParamOutput)		'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(6).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddAttributeFolderAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a attributeFolderAssignments user information.
'		Error True/False

Function SetAttributeFolderAssignmentUserCreated(AttributeFolderAssignmentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeFolderAssignmentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderAssignmentID", adInteger, adParamInput,, AttributeFolderAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeFolderAssignmentUserCreated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a AttributeFolderAssignments user information.
'		Error True/False

Function SetAttributeFolderAssignmentUserModified(AttributeFolderAssignmentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeFolderAssignmentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderAssignmentID", adInteger, adParamInput,, AttributeFolderAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeFolderAssignmentUserModified = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes an attribute from folder.
'		Error True/False

Function DeleteAttributeFolderAssignment(AttributeFolderAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeFolderAssignmentID)) and Not(IsNull(AttributeFolderAssignmentID)) and IsNumeric(AttributeFolderAssignmentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeFolderAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderAssignmentID", adInteger, adParamInput,, AttributeFolderAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeFolderAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates an AttributeFolderAssignment.
'		Error True/False

Function UpdateAttributeFolderAssignment(AttributeFolderAssignmentID, AttributeID, FolderID, Required, Comparable, Searchable)
	Dim returnValue, Cmd
	returnValue = True
	If Not(IsEmpty(AttributeFolderAssignmentID)) and Not(IsNull(AttributeFolderAssignmentID)) and Isnumeric(AttributeFolderAssignmentID) Then
		If Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and Isnumeric(AttributeID) Then
			If Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and Isnumeric(FolderID) Then
				If Not(IsEmpty(Required)) and Not(IsNull(Required)) and Isnumeric(Required) Then
					If Not(IsEmpty(Comparable)) and Not(IsNull(Comparable)) and Isnumeric(Comparable) Then
						If Not(IsEmpty(Searchable)) and Not(IsNull(Searchable)) and Isnumeric(Searchable) Then
							Call OpenSQLConnection(ctReadWrite)
							Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateAttributeFolderAssignment")
								Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderAssignmentID", adInteger, adParamInput,, AttributeFolderAssignmentID)	'1
								Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)									'2
								Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)											'3
								Cmd.Parameters.Append Cmd.CreateParameter("@Required", adBoolean, adParamInput,, Required)											'4
								Cmd.Parameters.Append Cmd.CreateParameter("@Comparable", adBoolean, adParamInput,, Comparable)										'5
								Cmd.Parameters.Append Cmd.CreateParameter("@Searchable", adBoolean, adParamInput,, Searchable)										'6
							Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
							If(Cmd.Parameters(0).Value = 0) Then
								returnValue = False
							End If
							Set Cmd = Nothing
							Call CloseSQLConnection(ctReadWrite)
						End If
					End If
				End If
			End If	
		End If
	End If
	UpdateAttributeFolderAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all attributes in the current folder.
'		Array/Null

Const aeAttributeFolderAssignmentID							= 0
Const aeAttributeFolderAssignmentAttributeID				= 1
Const aeAttributeFolderAssignmentAttributeKey				= 2
Const aeAttributeFolderAssignmentAttributeCode				= 3
Const aeAttributeFolderAssignmentAttributeName				= 4
Const aeAttributeFolderAssignmentAttributeDescription		= 5
Const aeAttributeFolderAssignmentAttributeTypeID			= 6
Const aeAttributeFolderAssignmentAttributeTypeKey			= 7
Const aeAttributeFolderAssignmentAttributeTypeName			= 8
Const aeAttributeFolderAssignmentAttributeTypeDescription	= 9
Const aeAttributeFolderAssignmentCollectionID				= 10
Const aeAttributeFolderAssignmentCollectionKey				= 11
Const aeAttributeFolderAssignmentCollectionName				= 12
Const aeAttributeFolderAssignmentCollectionDescription		= 13
Const aeAttributeFolderAssignmentFolderID					= 14
Const aeAttributeFolderAssignmentFolderKey					= 15
Const aeAttributeFolderAssignmentFolderCode					= 16
Const aeAttributeFolderAssignmentFolderSortOptionID			= 17
Const aeAttributeFolderAssignmentDirectoryID				= 18
Const aeAttributeFolderAssignmentAttributeFolderName		= 19
Const aeAttributeFolderAssignmentAttributeFolderDescription	= 20
Const aeAttributeFolderAssignmentAttributeFolderID			= 21
Const aeAttributeFolderAssignmentRequired					= 22
Const aeAttributeFolderAssignmentComparable					= 23
Const aeAttributeFolderAssignmentSearchable					= 24
Const aeAttributeFolderAssignmentFolderSortAssignmentID		= 25
Const aeAttributeFolderAssignmentSortOrder					= 26
Const aeAttributeFolderAssignmentUserModified				= 27
Const aeAttributeFolderAssignmentUserCreated				= 28
Const aeAttributeFolderAssignmentDateCreated				= 29
Const aeAttributeFolderAssignmentDateModified				= 30
Const aeAttributeFolderAssignmentKeyAttribute				= 31

Function GetAttributeFolderAssignments(FolderID, DirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeFolderAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeDirectoryID", adInteger, adParamInput,, DirectoryID)	'2
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeFolderAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all the folders this attribute is assigned to.
'		Array/Null

Const aeFolderAttributeAssignmentID							= 0
Const aeFolderAttributeAssignmentAttributeID				= 1
Const aeFolderAttributeAssignmentFolderID					= 2
Const aeFolderAttributeAssignmentFolderKey					= 3
Const aeFolderAttributeAssignmentFolderCode					= 4
Const aeFolderAttributeAssignmentFolderName					= 5
Const aeFolderAttributeAssignmentFolderDescription			= 6
Const aeFolderAttributeAssignmentolderSortOptionID			= 7
Const aeFolderAttributeAssignmentDirectoryID				= 8
Const aeFolderAttributeAssignmentDirectoryKey				= 9
Const aeFolderAttributeAssignmentDirectoryName				= 10
Const aeFolderAttributeAssignmentDirectoryDescription		= 11
Const aeFolderAttributeAssignmentRequired					= 12
Const aeFolderAttributeAssignmentComparable					= 13
Const aeFolderAttributeAssignmentSearchable					= 14
Const aeFolderAttributeAssignmentUserModified				= 15
Const aeFolderAttributeAssignmentUserCreated				= 16
Const aeFolderAttributeAssignmentDateCreated				= 17
Const aeFolderAttributeAssignmentDateModified				= 18

Function GetFolderAttributeAssignments(AttributeID, DirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetFolderAttributeAssignments")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetFolderAttributeAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the folder id the given attribute is under.
'		Returns UnitID/Null

Function GetAttributeFolderID(AttributeID, DirectoryID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and Len(AttributeID) > 0) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and Len(DirectoryID) > 0) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeFolderID")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)					'3
			Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetAttributeFolderID = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an attribute name from the Attribute Folder Assignment.
'		Returns AttributeName/Null

Function GetAttributeFolderAssignmentAttributeName(AttributeFolderAssignmentID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeFolderAssignmentID)) and Not(IsNull(AttributeFolderAssignmentID)) and IsNumeric(AttributeFolderAssignmentID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeFolderAssignmentAttributeName")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderAssignmentID", adInteger, adParamInput,, AttributeFolderAssignmentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeFolderAssignmentAttributeName = returnValue
End Function

'****************************************************************************************************************************
'*		Section Description:																								*
'*		Units																												*
'*			Wrappers for units		 																						*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an Unit.
'		Returns UnitID/Null

Function AddUnit(UnitCode, Name, Description, CollectionID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddUnit")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitCode", adVarChar, adParamInput, 50, UnitCode)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'4
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamOutput)								'5
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(5).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddUnit = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a Units user information.
'		Error True/False

Function SetUnitUserCreated(UnitID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetUnitUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetUnitUserCreated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a Units user information.
'		Error True/False

Function SetUnitUserModified(UnitID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetUnitUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetUnitUserModified = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an Unit.
'		Error True/False

Function DeleteUnit(UnitID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteUnit")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUnit = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates an Unit.
'		Error True/False

Function UpdateUnit(UnitID, Name, Description, CollectionID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateUnit")
				Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)		'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	UpdateUnit = returnValue
End Function

Function UpdateUnit2(UnitID, Name, Description, CollectionID, NormalizeFactor)
	Dim returnValue, Cmd, Par
	returnValue = True
	
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID) and IsNumeric(NormalizeFactor)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then			    
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateUnit2")
				Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)					'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)		'4
				Set Par = Cmd.CreateParameter("@NormalizeFactor", adNumeric, adParamInput,, NormalizeFactor)	'5
				Par.Precision = 19 
				Par.NumericScale = 6
				Cmd.Parameters.Append Par
				
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				
				If(Cmd.Parameters(0).Value <> 0) Then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	UpdateUnit2 = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an Unit.
'		Array/Null

Const aeUnitID				= 0
Const aeUnitKey				= 1
Const aeUnitCode			= 2
Const aeUnitName			= 3
Const aeUnitDescription		= 4
Const aeUnitCollectionID	= 5
Const aeUnitUserModified	= 6
Const aeUnitUserCreated		= 7
Const aeUnitDateCreated		= 8
Const aeUnitDateModified	= 9
Const aeUnitNormalizeFactor	= 10

Function GetUnit(UnitID)
	Dim returnValue, ary(9), Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnit")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitKey", adChar, adParamOutput, 32)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitCode", adVarChar, adParamOutput, 15)		'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)	'5
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)		'6
		Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)	'7
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)		'8
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)		'9
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)	'10
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeUnitID)				= UnitID
			ary(aeUnitKey)				= Cmd.Parameters(2).Value
			ary(aeUnitCode)				= Cmd.Parameters(3).Value
			ary(aeUnitName)				= Cmd.Parameters(4).Value
			ary(aeUnitDescription)		= Cmd.Parameters(5).Value
			ary(aeUnitCollectionID)		= Cmd.Parameters(6).Value
			ary(aeUnitUserModified)		= Cmd.Parameters(7).Value
			ary(aeUnitUserCreated)		= Cmd.Parameters(8).Value
			ary(aeUnitDateCreated)		= Cmd.Parameters(9).Value
			ary(aeUnitDateModified)		= Cmd.Parameters(10).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnit = returnValue
End Function

Function GetUnit2(UnitID)
	Dim returnValue, ary(10), Cmd, Par
	returnValue = Null
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnit2")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitKey", adChar, adParamOutput, 32)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitCode", adVarChar, adParamOutput, 15)		'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)		'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)	'5
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)		'6
		Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)	'7
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)		'8
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)		'9
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)	'10
		Set Par = Cmd.CreateParameter("@NormalizeFactor", adNumeric, adParamOutput)			'11
		Par.Precision = 19 
		Par.NumericScale = 6
		Cmd.Parameters.Append Par
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeUnitID)				= UnitID
			ary(aeUnitKey)				= Cmd.Parameters(2).Value
			ary(aeUnitCode)				= Cmd.Parameters(3).Value
			ary(aeUnitName)				= Cmd.Parameters(4).Value
			ary(aeUnitDescription)		= Cmd.Parameters(5).Value
			ary(aeUnitCollectionID)		= Cmd.Parameters(6).Value
			ary(aeUnitUserModified)		= Cmd.Parameters(7).Value
			ary(aeUnitUserCreated)		= Cmd.Parameters(8).Value
			ary(aeUnitDateCreated)		= Cmd.Parameters(9).Value
			ary(aeUnitDateModified)		= Cmd.Parameters(10).Value
			ary(aeUnitNormalizeFactor)	= Cmd.Parameters(11).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnit2 = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a UnitID from the guid.
'		Returns UnitID/Null

Function GetUnitID(UnitKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitKey)) and Not(IsNull(UnitKey)) and Len(UnitKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitID")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitKey", adChar, adParamInput, 32, UnitKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a UnitID from the UnitCode.
'		Returns UnitID/Null

Function GetUnitIDByCode(UnitCode)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitCode)) and Not(IsNull(UnitCode)) and Len(UnitCode) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitCode", adVarChar, adParamInput, 15, UnitCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the Unit information
'		Returns UnitID/Null

const aeUnitsUnitID				= 0
const aeUnitsUnitKey			= 1
const aeUnitsUnitCode			= 2
const aeUnitsUnitName			= 3
const aeUnitsUnitDescription	= 4
const aeUnitsUnitCollectionID	= 5
const aeUnitsUnitUserModified	= 6
const aeUnitsUnitUserCreated	= 7
const aeUnitsUnitDateCreated	= 8
const aeUnitsUnitDateModified	= 9

Function GetUnits(CollectionID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnits")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnits = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a unit name.
'		Returns UnitName/Null

Function GetUnitName(UnitID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitName")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)			'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitName = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the unassociated Units
'		Returns UnitID/Null

const aeUAUnitsUnitID				= 0
const aeUAUnitsUnitKey				= 1
const aeUAUnitsUnitCode				= 2
const aeUAUnitsUnitName				= 3
const aeUAUnitsUnitDescription		= 4
const aeUAUnitsUnitCollectionID		= 5
const aeUAUnitsUnitUserModified		= 6
const aeUAUnitsUnitUserCreated		= 7
const aeUAUnitsUnitDateCreated		= 8
const aeUAUnitsUnitDateModified		= 9

Function GetUnassociatedUnits()
	Dim returnValue
	returnValue = null
	Dim Cmd
	Dim resultSet
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnassociatedUnits")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If not resultSet.EOF Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetUnassociatedUnits = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the folder id the given unit is under.
'		Returns UnitID/Null

Function GetUnitFolderID(UnitID, DirectoryID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitFolderID")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)					'3
			Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetUnitFolderID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the attributes using this unit.
'		Array/Null

Const aeUnitAttributeAttributeID						= 0
Const aeUnitAttributeAttributeKey						= 1
Const aeUnitAttributeAttributeCode						= 2
Const aeUnitAttributeAttributeName						= 3
Const aeUnitAttributeAttributeDescription				= 4
Const aeUnitAttributeAttributeAttributeTypeID			= 5
Const aeUnitAttributeAttributeAttributeTypeKey			= 6
Const aeUnitAttributeAttributeAttributeTypeName			= 7
Const aeUnitAttributeAttributeAttributeTypeDescription	= 8
Const aeUnitAttributeAttributeCollectionID				= 9
Const aeUnitAttributeAttributeCollectionKey				= 10
Const aeUnitAttributeAttributeCollectionName			= 11
Const aeUnitAttributeAttributeCollectionDescription		= 12
Const aeUnitAttributeAttributeUserModified				= 13
Const aeUnitAttributeAttributeUserCreated				= 14
Const aeUnitAttributeAttributeDateCreated				= 15
Const aeUnitAttributeAttributeDateModified				= 16

Function GetUnitAttributes(UnitID, CollectionID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitAttributes")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetUnitAttributes = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Unit Folder Assignments																								*
'*			Wrappers for Unit Folder Assignments																			*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an Unit to a folder.
'		Returns UnitFolderAssignmentID/Null

Function AddUnitFolderAssignment(unitID, FolderID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(unitID)) and Not(IsNull(unitID)) and IsNumeric(unitID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddUnitFolderAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, unitID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@unitFolderAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddunitFolderAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an Unit from folder.
'		Error True/False

Function DeleteUnitFolderAssignment(UnitFolderAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(UnitFolderAssignmentID)) and Not(IsNull(UnitFolderAssignmentID)) and IsNumeric(UnitFolderAssignmentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteUnitFolderAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitFolderAssignmentID", adInteger, adParamInput,, UnitFolderAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUnitFolderAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all Units in the current folder.
'		Array/Null

Const aeUnitFolderAssignmentID					= 0
Const aeUnitFolderAssignmentUnitID				= 1
Const aeUnitFolderAssignmentUnitKey				= 2
Const aeUnitFolderAssignmentUnitCode			= 3
Const aeUnitFolderAssignmentUnitName			= 4
Const aeUnitFolderAssignmentUnitDescription		= 5
Const aeUnitFolderAssignmentUnitCollectionID	= 6
Const aeUnitFolderAssignmentDateCreated			= 7
Const aeUnitFolderAssignmentDateModified		= 8

Function GetUnitFolderAssignments(FolderID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitFolderAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitFolderAssignments = returnValue
End Function

Const aeUnitFolderAssignmentNormalizeFactor		= 9
Function GetUnitFolderAssignments2(FolderID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitFolderAssignments2")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitFolderAssignments2 = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a unit folder location (complete path to the root).
'		Returns Array/Null

Const aeUnitLocationFolderID			= 0
Const aeUnitLocationFolderKey			= 1
Const aeUnitLocationFolderCode			= 2
Const aeUnitLocationFolderDirectoryID	= 3
Const aeUnitLocationFolderName			= 4
Const aeUnitLocationFolderDescription	= 5
Const aeUnitLocationFolderDateCreated	= 6
Const aeUnitLocationFolderDateModified	= 7

Function GetUnitLocation(UnitID)
	Dim returnValue, resultSet, ary, tempary, i, Cmd
	returnValue = Null
	i = 0
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitLocation")
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
						ary(aeUnitLocationFolderID,i)			= tempary(aeUnitLocationFolderID,0)
						ary(aeUnitLocationFolderKey,i)			= tempary(aeUnitLocationFolderKey,0)
						ary(aeUnitLocationFolderCode,i)			= tempary(aeUnitLocationFolderCode,0)
						ary(aeUnitLocationFolderDirectoryID,i)	= tempary(aeUnitLocationFolderDirectoryID,0)
						ary(aeUnitLocationFolderName,i)			= tempary(aeUnitLocationFolderName,0)
						ary(aeUnitLocationFolderDescription,i)	= tempary(aeUnitLocationFolderDescription,0)
						ary(aeUnitLocationFolderDateCreated,i)	= tempary(aeUnitLocationFolderDateCreated,0)
						ary(aeUnitLocationFolderDateModified,i)	= tempary(aeUnitLocationFolderDateModified,0)
					End If
				End If
			End If
			Set resultSet = resultSet.NextRecordset
			i = i + 1
		Wend
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
		returnValue = ary
	End If
	GetUnitLocation = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an unit name from the Unit Folder Assignment.
'		Returns UnitName/Null

Function GetUnitFolderAssignmentUnitName(UnitFolderAssignmentID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitFolderAssignmentID)) and Not(IsNull(UnitFolderAssignmentID)) and IsNumeric(UnitFolderAssignmentID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitFolderAssignmentUnitName")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitFolderAssignmentID", adInteger, adParamInput,, UnitFolderAssignmentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitFolderAssignmentUnitName = returnValue
End Function

'****************************************************************************************************************************
'*		Section Description:																								*
'*		Attribute Value Product Assignments																					*
'*			Wrappers for attribute value product assignments																*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an attribute value product assignment.
'		Returns AttributeValueProductAssignmentID/Null

Function AddAttributeValueProductAssignment(AttributeID, ProductID, AttributeValueID, AttributeStatusID, UserName)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			If(Not(IsEmpty(AttributeValueID)) and Not(IsNull(AttributeValueID)) and IsNumeric(AttributeValueID)) Then
				If(Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
					If(IsEmpty(UserName) OR IsNull(UserName) OR Not(Len(UserName) > 0)) Then UserName = "System Account" End If
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeValueProductAssignment")
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'2
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)		'3
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'4
					Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput,50, UserName)					'5
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueProductAssignmentID", adInteger, adParamOutput)		'6
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If(Cmd.Parameters(0).Value = 0) Then
						returnValue = Cmd.Parameters(6).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	AddAttributeValueProductAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a attributeValueProductAssignments user information.
'		Error True/False

Function SetAttributeValueProductAssignmentUserCreated(AttributeValueProductAssignmentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeValueProductAssignmentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueProductAssignmentID", adInteger, adParamInput,, AttributeValueProductAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeValueProductAssignmentUserCreated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a AttributeValueProductAssignments user information.
'		Error True/False

Function SetAttributeValueProductAssignmentUserModified(AttributeValueProductAssignmentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetAttributeValueProductAssignmentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueProductAssignmentID", adInteger, adParamInput,, AttributeValueProductAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetAttributeValueProductAssignmentUserModified = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an attributeValueProduct assignment.
'		Error True/False

Function DeleteAttributeValueProductAssignment(AttributeValueProductAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeValueProductAssignmentID)) and Not(IsNull(AttributeValueProductAssignmentID)) and IsNumeric(AttributeValueProductAssignmentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeValueProductAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueProductAssignmentID", adInteger, adParamInput,, AttributeValueProductAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeValueProductAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all attribute value product assignments for the current attribute.
'		Array/Null

Const aeAttributeValueProductAssignmentID							= 0
Const aeAttributeValueProductAssignmentAttributeID					= 1
Const aeAttributeValueProductAssignmentAttributeKey					= 2
Const aeAttributeValueProductAssignmentAttributeCode				= 3
Const aeAttributeValueProductAssignmentName							= 4
Const aeAttributeValueProductAssignmentDescription					= 5
Const aeAttributeValueProductAssignmentCollectionID					= 6
Const aeAttributeValueProductAssignmentCollectionKey				= 7
Const aeAttributeValueProductAssignmentCollectionName				= 8
Const aeAttributeValueProductAssignmentCollectionDescription		= 9
Const aeAttributeValueProductAssignmentAttributeTypeID				= 10
Const aeAttributeValueProductAssignmentAttributeTypeKey				= 11
Const aeAttributeValueProductAssignmentAttributeTypeName			= 12
Const aeAttributeValueProductAssignmentAttributeTypeDescription		= 13
Const aeAttributeValueProductAssignmentProductID					= 14
Const aeAttributeValueProductAssignmentAttributeValueID				= 15
Const aeAttributeValueProductAssignmentAttributeValueKey			= 16
Const aeAttributeValueProductAssignmentAttributeValueCode			= 17
Const aeAttributeValueProductAssignmentAttributeValueValue			= 18
Const aeAttributeValueProductAssignmentAttributeValueSearchableValue= 19
Const aeAttributeValueProductAssignmentAttributeValueNumericValue	= 20
Const aeAttributeValueProductAssignmentAttributeValueUnitID			= 21
Const aeAttributeValueProductAssignmentAttributeValueActive			= 22
Const aeAttributeValueProductAssignmentAttributeStatusID			= 23
Const aeAttributeValueProductAssignmentAttributeStatusKey			= 24
Const aeAttributeValueProductAssignmentAttributeStatusName			= 25
Const aeAttributeValueProductAssignmentAttributeStatusDescription	= 26
Const aeAttributeValueProductAssignmentUserModified					= 27
Const aeAttributeValueProductAssignmentUserCreated					= 28
Const aeAttributeValueProductAssignmentDateCreated					= 29
Const aeAttributeValueProductAssignmentDateModified					= 30

Function GetAttributeValueProductAssignments(AttributeID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeValueProductAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeValueProductAssignments = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Unit Maps																											*
'*			Wrappers for unit maps		 																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an Unit map.
'		Returns UnitMapID/Null

Function AddUnitMap(UnitID, MappedUnitID)
	Dim returnValue, Cmd
	returnValue = Null
	If Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID) Then
		If Not(IsEmpty(MappedUnitID)) and Not(IsNull(MappedUnitID)) and IsNumeric(MappedUnitID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddUnitMap")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@MappedUnitID", adInteger, adParamInput,, MappedUnitID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitMapID", adInteger, adParamOutput)						'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddUnitMap = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an Unit Map.
'		Error True/False

Function DeleteUnitMap(UnitMapID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(UnitMapID)) and Not(IsNull(UnitMapID)) and IsNumeric(UnitMapID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteUnitMap")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitMapID", adInteger, adParamInput,, UnitMapID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUnitMap = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns unit list containing map units.
'		Array/Null

Const aeUnitMapID					= 0
Const aeUnitMapUnitID				= 1
Const aeUnitMapUnitKey				= 2
Const aeUnitMapUnitCode				= 3
Const aeUnitMapUnitName				= 4
Const aeUnitMapUnitDescription		= 5
Const aeUnitMapUnitCollectionID		= 6
Const aeUnitMapUnitUserModified		= 7
Const aeUnitMapUnitUserCreated		= 8
Const aeUnitMapMappedUnitID			= 9
Const aeUnitMapMappedUnitKey			= 10
Const aeUnitMapMappedUnitCode			= 11
Const aeUnitMapMappedUnitName			= 12
Const aeUnitMapMappedUnitDescription	= 13
Const aeUnitMapMappedUnitCollectionID	= 14
Const aeUnitMapMappedUnitUserModified	= 15
Const aeUnitMapMappedUnitUserCreated	= 16
Const aeUnitMapDateCreated			= 17
Const aeUnitMapDateModified			= 18

Function GetUnitMaps(UnitID, DirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitMap")
			Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetUnitMaps = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Product Comments																									*
'*			Wrappers for Product Comments 																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an Product Comment.
'		Returns ProductCommentID/Null

Function AddProductComment(ProductID, Comment)
	Dim returnValue, Cmd
	returnValue = Null
	If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) Then
		If Not(IsEmpty(Comment)) and Not(IsNull(Comment)) and Len(Comment) > 0  Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddProductComment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Comment", adVarchar, adParamInput,8000, Comment)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductCommentID", adInteger, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductComment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates a ProductComments user information.
'		Error True/False

Function SetProductCommentUserCreated(ProductCommentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetProductCommentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductCommentID", adInteger, adParamInput,, ProductCommentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarChar, adParamInput, 50, UserName)	'2

			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetProductCommentUserCreated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a ProductComments user information.
'		Error True/False

Function SetProductCommentUserModified(ProductCommentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetProductCommentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductCommentID", adInteger, adParamInput,, ProductCommentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)			'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetProductCommentUserModified = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an Product Comment.
'		Error True/False

Function DeleteProductComment(ProductCommentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductCommentID)) and Not(IsNull(ProductCommentID)) and IsNumeric(ProductCommentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteProductComment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCommentID", adInteger, adParamInput,, ProductCommentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductComment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an Product Comment.
'		Array/Null

Const aeProductCommentProductCommentID				= 0
Const aeProductCommentProductID						= 1
Const aeProductCommentComment						= 2
Const aeProductCommentUserModified					= 3
Const aeProductCommentUserCreated					= 4
Const aeProductCommentProductCommentDateCreated		= 5
Const aeProductCommentProductCommentDateModified	= 6

Function GetProductComment(ProductCommentID)
	Dim returnValue, ary(6), Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductCommentID)) and Not(IsNull(ProductCommentID)) and IsNumeric(ProductCommentID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductComment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCommentID", adInteger, adParamInput,, ProductCommentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Comment", adVarChar, adParamOutput, 8000)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@UserModifed", adVarChar, adParamOutput, 50)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)						'6
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)					'7
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeProductCommentProductCommentID)			= ProductCommentID
			ary(aeProductCommentProductID)					= Cmd.Parameters(2).Value
			ary(aeProductCommentComment)					= Cmd.Parameters(3).Value
			ary(aeProductCommentUserModified)				= Cmd.Parameters(4).Value
			ary(aeProductCommentUserCreated)				= Cmd.Parameters(5).Value
			ary(aeProductCommentProductCommentDateCreated)	= Cmd.Parameters(6).Value
			ary(aeProductCommentProductCommentDateModified)	= Cmd.Parameters(7).Value

			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitMap = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all ProductComments for the current Product.
'		Array/Null

Const aeProductCommentsProductID					= 0
Const aeProductCommentsProductCommentID				= 1
Const aeProductCommentsComment						= 2
Const aeProductCommentsUserModified					= 3
Const aeProductCommentsUserCreated					= 4
Const aeProductCommentsProductCommentDateCreated	= 5
Const aeProductCommentsProductCommentDateModified	= 6

Function GetProductComments(ProductID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductComments")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductComments = returnValue
End Function

'****************************************************************************************************************************
'*		Section Description:																								*
'*		Attribute Maps																										*
'*			Wrappers for Attribute maps	 																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an Attribute map.
'		Returns AttributeMapID/Null

Function AddAttributeMap(AttributeID, MappedAttributeID)
	Dim returnValue, Cmd
	returnValue = Null
	If Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID) Then
		If Not(IsEmpty(MappedAttributeID)) and Not(IsNull(MappedAttributeID)) and IsNumeric(MappedAttributeID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeMap")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@MappedAttributeID", adInteger, adParamInput,, MappedAttributeID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeMapID", adInteger, adParamOutput)								'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddAttributeMap = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes an Attribute Map.
'		Error True/False

Function DeleteAttributeMap(AttributeMapID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeMapID)) and Not(IsNull(AttributeMapID)) and IsNumeric(AttributeMapID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeMap")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeMapID", adInteger, adParamInput,, AttributeMapID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeMap = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an Attribute map.
'		Array/Null

Const aeAttributeMapsAttributeMapID				= 0
Const aeAttributeMapsAttributeID				= 1
Const aeAttributeMapsAttributeKey				= 2
Const aeAttributeMapsAttributeCode				= 3
Const aeAttributeMapsAttributeName				= 4
Const aeAttributeMapsAttributeDescription		= 5
Const aeAttributeMapsAttributeCollectionID		= 6
Const aeAttributeMapsAttributeTypeID			= 7
Const aeAttributeMapsAttributeUserModified		= 8
Const aeAttributeMapsAttributeUserCreated		= 9
Const aeAttributeMapsMappedAttributeID			= 10
Const aeAttributeMapsMappedAttributeKey			= 11
Const aeAttributeMapsMappedAttributeCode		= 12
Const aeAttributeMapsMappedAttributeName		= 13
Const aeAttributeMapsMappedAttributeDescription	= 14
Const aeAttributeMapsMappedAttributeCollectionID= 15
Const aeAttributeMapsMappedAttributeTypeID		= 16
Const aeAttributeMapsMappedUserModified			= 17
Const aeAttributeMapsMappedUserCreated			= 18
Const aeAttributeMapsAttributeDateCreated		= 19
Const aeAttributeMapsAttributeDateModified		= 20

Function GetAttributeMaps(AttributeID, DirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeMap")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)		'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetAttributeMaps = returnValue
End Function

'****************************************************************************************************************************
'*		Section Description:																								*
'*		Gets the KeyAttribute flag for a particular attribute belonging to a Pgm																											*
'*																									*
'****************************************************************************************************************************
'	
'	DESCRIPTION:
'		Returns a AttributeID from the AttributeCode.
'		Returns AttributeID/Null

Function GetAttributeKeyFlag(AttributeID,PgmID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and Len(AttributeID) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeKeyFlag")
		Cmd.Parameters.Append Cmd.CreateParameter("@PgmID", adInteger, adParamInput,, PgmID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)
		Cmd.Parameters.Append Cmd.CreateParameter("@KeyFlag", adBoolean, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeKeyFlag = returnValue
End Function



'****************************************************************************************************************************
'*		Section Description:																								*
'*		AttributeValueMaps																									*
'*			Wrappers for AttributeValueMaps	 																				*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an AttributeValueMap.
'		Returns AttributeMapValueID/Null

Function AddAttributeValueMap(AttributeValueID, MappedAttributeValueID)
	Dim returnValue, Cmd
	returnValue = Null
	If Not(IsEmpty(AttributeValueID)) and Not(IsNull(AttributeValueID)) and IsNumeric(AttributeValueID) Then
		If Not(IsEmpty(MappedAttributeValueID)) and Not(IsNull(MappedAttributeValueID)) and IsNumeric(MappedAttributeValueID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeValueMap")
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueID", adInteger, adParamInput,, AttributeValueID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@MappedAttributeValueID", adInteger, adParamInput,, MappedAttributeValueID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueMapID", adInteger, adParamOutput)									'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddAttributeValueMap = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an AttributeValueMap.
'		Error True/False

Function DeleteAttributeValueMap(AttributeValueMapID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(AttributeValueMapID)) and Not(IsNull(AttributeValueMapID)) and IsNumeric(AttributeValueMapID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteAttributeValueMap")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueMapID", adInteger, adParamInput,, AttributeValueMapID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteAttributeValueMap = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Unit Conversions																									*
'*			Wrappers for Unit Conversions 																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds a Unit Conversion.
'		Returns UnitConversionID/Null

Function AddUnitConversion(UnitID, ConvertedUnitID, ConversionValue)
	Dim returnValue, Cmd
	returnValue = Null
	If Not(IsEmpty(UnitID)) and Not(IsNull(UnitID)) and IsNumeric(UnitID) Then
		If Not(IsEmpty(ConvertedUnitID)) and Not(IsNull(ConvertedUnitID)) and IsNumeric(ConvertedUnitID) Then
			If Not(IsEmpty(ConversionValue)) and Not(IsNull(ConversionValue)) and Len(ConversionValue) > 0  Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddAttributeMap")
				Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamInput,, UnitID)							'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ConvertedUnitID", adInteger, adParamInput,, ConvertedUnitID)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@ConversionValue", adDouble, adParamInput,53, ConversionValue)		'3
				Cmd.Parameters.Append Cmd.CreateParameter("@UnitConversionID", adInteger, adParamOutput)						'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddUnitConversion = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an Unit Conversion.
'		Error True/False

Function DeleteUnitConversion(UnitConversionID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(UnitConversionID)) and Not(IsNull(UnitConversionID)) and IsNumeric(UnitConversionID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteUnitConversion")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitConversionID", adInteger, adParamInput,, UnitConversionID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUnitConversion = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an Unit Conversion.
'		Array/Null

Const aeUnitConversionsUnitConversionID			= 0
Const aeUnitConversionsUnitID					= 1
Const aeUnitConversionsUnitKey					= 2
Const aeUnitConversionsUnitCode					= 3
Const aeUnitConversionsUnitName					= 4
Const aeUnitConversionsUnitDescription			= 5
Const aeUnitConversionsConvertedUnitID			= 6
Const aeUnitConversionsConvertedUnitKey			= 7
Const aeUnitConversionsConvertedUnitCode		= 8
Const aeUnitConversionsConvertedUnitName		= 9
Const aeUnitConversionsConvertedUnitDescription	= 10
Const aeUnitConversionsConversionValue			= 11
Const aeUnitConversionsDateCreated				= 12
Const aeUnitConversionsDateModified				= 13

Function GetUnitConversion(UnitConversionID)
	Dim returnValue, ary(13), Cmd
	returnValue = Null
	If(Not(IsEmpty(UnitConversionID)) and Not(IsNull(UnitConversionID)) and IsNumeric(UnitConversionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetUnitConversion")
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitConversionID", adInteger, adParamInput,, UnitConversionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitID", adInteger, adParamOutput)								'2
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitKey", adChar, adParamOutput, 32)							'3
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitCode", adVarChar, adParamOutput, 15)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitName", adVarChar, adParamOutput, 75)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@UnitDescription", adVarChar, adParamOutput, 500)				'6
		Cmd.Parameters.Append Cmd.CreateParameter("@ConvertedUnitID", adInteger, adParamOutput)						'7
		Cmd.Parameters.Append Cmd.CreateParameter("@ConvertedUnitKey", adChar, adParamOutput, 32)					'8
		Cmd.Parameters.Append Cmd.CreateParameter("@ConvertedUnitCode", adVarChar, adParamOutput, 15)				'9
		Cmd.Parameters.Append Cmd.CreateParameter("@ConvertedUnitName", adVarChar, adParamOutput, 75)				'10
		Cmd.Parameters.Append Cmd.CreateParameter("@ConvertedUnitDescription", adVarChar, adParamOutput, 500)		'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ConversionValue", adDouble, adParamOutput)						'12
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)						'13
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)					'14
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeUnitConversionsUnitConversionID)			= UnitConversionID
			ary(aeUnitConversionsUnitID)					= Cmd.Parameters(2).Value
			ary(aeUnitConversionsUnitKey)					= Cmd.Parameters(3).Value
			ary(aeUnitConversionsUnitCode)					= Cmd.Parameters(4).Value
			ary(aeUnitConversionsUnitName)					= Cmd.Parameters(5).Value
			ary(aeUnitConversionsUnitDescription)			= Cmd.Parameters(6).Value
			ary(aeUnitConversionsConvertedUnitID)			= Cmd.Parameters(7).Value
			ary(aeUnitConversionsConvertedUnitKey)			= Cmd.Parameters(8).Value
			ary(aeUnitConversionsConvertedUnitCode)			= Cmd.Parameters(9).Value
			ary(aeUnitConversionsConvertedUnitName)			= Cmd.Parameters(10).Value
			ary(aeUnitConversionsConvertedUnitDescription)	= Cmd.Parameters(11).Value
			ary(aeUnitConversionsConversionValue)			= Cmd.Parameters(12).Value
			ary(aeUnitConversionsDateCreated)				= Cmd.Parameters(13).Value
			ary(aeUnitConversionsDateModified)				= Cmd.Parameters(14).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetUnitConversion = returnValue
End Function


'************************************************************************************************************************
'	TRANSACTIVE RULE ASSIGNMENTS																						'
'		The following section houses all of the wrappers for transactive rule assignments								'
'		Add, Delete and Update of rules for Product Family, Product Folder, Product and Manufacture						'
'		Also gets the rule assignments and check if an assignment already exists										'
'************************************************************************************************************************
'	DESCRIPTION:
'		Add a product family rule assignment.
'		Array/Null

Function AddProductFamilyRuleAssignment(ProductFamilyID, RuleID)
	Dim returnValue, Cmd
	returnValue = null
	If(Not(IsEmpty(ProductFamilyID)) and Not(IsNull(ProductFamilyID)) and IsNumeric(ProductFamilyID)) Then
		If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_AddProductFamilyRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyID", adInteger, adParamInput,, ProductFamilyID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyRuleActionAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductFamilyRuleAssignment = returnValue
End Function


'************************************************************************************************************************
' Update a rule for a product family. 

Function UpdateProductFamilyRuleAssignment(ProductFamilyRuleActionAssignmentID, RuleID)
	Dim returnValue
	returnValue = true
	If not isEmpty(ProductFamilyRuleActionAssignmentID) and not isNull(ProductFamilyRuleActionAssignmentID) and IsNumeric(ProductFamilyRuleActionAssignmentID) Then
		If not isEmpty(RuleID) and not isNull(RuleID) and IsNumeric(RuleID) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_UpdateProductFamilyRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyRuleActionAssignmentID", adInteger, adParamInput,, ProductFamilyRuleActionAssignmentID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = false
			End If
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateProductFamilyRuleAssignment = returnValue
End Function


'************************************************************************************************************************
Function DeleteProductFamilyRuleAssignment(ProductFamilyRuleActionAssignmentID)
	Dim returnValue
	returnValue = null
	If not isEmpty(ProductFamilyRuleActionAssignmentID) and not isNull(ProductFamilyRuleActionAssignmentID) and IsNumeric(ProductFamilyRuleActionAssignmentID) Then
		Dim Cmd
		call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_DeleteProductFamilyRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyRuleActionAssignmentID", adInteger, adParamInput,, ProductFamilyRuleActionAssignmentID)	'1
		call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
		set Cmd = Nothing
		call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductFamilyRuleAssignment = returnValue
End Function


'************************************************************************************************************************
'	DESCRIPTION:
'		Get the RuleID for an Product family
'		Returns RuleID/Null

Function GetProductFamilyRuleAssignment(ProductFamilyID)
	Dim returnValue
	returnValue = null
	If not isEmpty(ProductFamilyID) and not isNull(ProductFamilyID) and IsNumeric(ProductFamilyID) Then
		Dim Cmd
		call OpenSQLConnection(ConnectionType_Attribute)
		set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductFolderRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyID", adInteger, adParamInput,, ProductFamilyID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyRuleActionAssignmentID", adInteger, adParamOutput)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamOutput) 								'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(0)		= ProductFamilyID
			ary(1)		= Cmd.Parameters(2).Value
			ary(2)		= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		set Cmd = Nothing
		call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductFamilyRuleAssignment = returnValue
End Function


'************************************************************************************************************************
'	DESCRIPTION:
'		Check if the Product Family has a transactive rule assignment
'		True/False

Function HasProductFamilyRuleActionAssignment(ProductFamilyID, RuleID)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(ProductFamilyID) and Not IsNull(ProductFamilyID) and IsNumeric(ProductFamilyID) Then
		If Not IsEmpty(RuleID) and Not IsNull(RuleID) and IsNumeric(RuleID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_HasProductFamilyRuleActionAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@ProductFamilyID", adInteger, adParamInput,, Product)				'1
				cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)							'2
				cmd.Parameters.Append Cmd.CreateParameter("@HasProductFamilyRuleActionAssignment", adBoolean, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasProductFamilyRuleActionAssignment = ReturnValue
		End If
	End If
End Function


'************************************************************************************************************************
'	DESCRIPTION:
'		Add a product folder rule assignment.
'		Array/Null

Function AddProductFolderRuleAssignment(FolderID, RuleID)
	Dim returnValue, Cmd
	returnValue = null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_AddProductFolderRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderRuleAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductFolderRuleAssignment = returnValue
End Function


'************************************************************************************************************************
' Update a rule for a product folder. 

Function UpdateProductFolderRuleAssignment(ProductFolderRuleAssignmentID, RuleID)
	Dim returnValue
	returnValue = true
	If not isEmpty(ProductFolderRuleAssignmentID) and not isNull(ProductFolderRuleAssignmentID) and IsNumeric(ProductFolderRuleAssignmentID) Then
		If not isEmpty(RuleID) and not isNull(RuleID) and IsNumeric(RuleID) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_UpdateProductFolderRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderRuleAssignmentID", adInteger, adParamInput,, ProductFolderRuleAssignmentID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = false
			End If
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateProductFolderRuleAssignment = returnValue
End Function


'************************************************************************************************************************
Function DeleteProductFolderRuleAssignment(ProductFolderAssignmentID)
	Dim returnValue
	returnValue = null
	If not isEmpty(ProductFolderAssignmentID) and not isNull(ProductFolderAssignmentID) and IsNumeric(ProductFolderAssignmentID) Then
		Dim Cmd
		call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_DeleteProductFolderRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderAssignmentID", adInteger, adParamInput,, ProductFolderAssignmentID)	'1
		call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
		set Cmd = Nothing
		call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductFolderRuleAssignment = returnValue
End Function


'************************************************************************************************************************
'	DESCRIPTION:
'		Get the RuleID for an Product folder
'		Returns RuleID/Null

Function GetProductFolderRuleAssignment(folderID)
	Dim returnValue
	returnValue = null
	If not isEmpty(folderID) and not isNull(folderID) and IsNumeric(folderID) Then
		Dim Cmd
		call OpenSQLConnection(ConnectionType_Attribute)
		set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductFolderRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@folderID", adInteger, adParamInput,, folderID)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderRuleAssignmentID", adInteger, adParamOutput)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamOutput)								'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(0)		= folderID
			ary(1)		= Cmd.Parameters(2).Value
			ary(2)		= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		set Cmd = Nothing
		call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductFolderRuleAssignment = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Get the rules for Product folders based on the RuleID
'		Returns RuleID/Null

Const trProductFolderRuleProductFolderRuleAssignmentID	= 0
Const trProductFolderRuleRuleID							= 1
Const trProductFolderRuleFolderID						= 2
Const trProductFolderRuleFolderCode						= 3
Const trProductFolderRuleDirectoryID					= 4
Const trProductFolderRuleFolderName						= 5
Const trProductFolderRuleFolderDescription				= 6
Const trProductFolderRuleDateCreated					= 7
Const trProductFolderRuleDateModified					= 8

Function GetTransactiveProductFolderRuleAssignments(RuleID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductFolderRuleAssignmentsByRuleID")
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)		'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetTransactiveProductFolderRuleAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a Product Folder Name from a ProductFolderRuleAssignmentID.
'		Returns Product Folder Name/Null

Function GetProductFolderRuleAssignmentName(ProductFolderRuleAssignmentID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductFolderRuleAssignmentID)) and Not(IsNull(ProductFolderRuleAssignmentID)) and Len(ProductFolderRuleAssignmentID) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductFolderRuleAssignmentName")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderRuleAssignmentID", adInteger, adParamInput,, ProductFolderRuleAssignmentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput, 75)														'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductFolderRuleAssignmentName = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Check if the Product Folder has a transactive rule assignment
'		True/False

Function HasProductFolderRuleAssignment(FolderID, RuleID)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(FolderID) and Not IsNull(FolderID) and IsNumeric(FolderID) Then
		If Not IsEmpty(RuleID) and Not IsNull(RuleID) and IsNumeric(RuleID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_HasProductFolderRuleAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
				cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)				'2
				cmd.Parameters.Append Cmd.CreateParameter("@HasProductRuleAssignment", adBoolean, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasProductFolderRuleAssignment = ReturnValue
		End If
	End If
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a product Approvement.
'		Error True/False

Function UpdateProductApprovement(ProductID, MovedToProduction)
	Dim returnValue, Cmd
	returnValue = True
	If (Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and Len(ProductID) > 0) Then
		If (Not(IsEmpty(MovedToProduction)) and Not(IsNull(MovedToProduction)) and IsNumeric(MovedToProduction)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_UpdateProductApprovement")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@MovedToProduction", adInteger, adParamInput,, MovedToProduction)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateProductApprovement = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Add a product rule assignment.
'		Array/Null

Function AddProductRuleAssignment(ProductID, RuleID)
	Dim returnValue, Cmd
	returnValue = null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_AddProductRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductActionAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductRuleAssignment = returnValue
End Function


'************************************************************************************************************************
' Update a rule for a product. 

Function UpdateProductRuleAssignment(ProductActionAssignmentID, RuleID)
	Dim returnValue
	returnValue = true
	If not isEmpty(ProductActionAssignmentID) and not isNull(ProductActionAssignmentID) and IsNumeric(ProductActionAssignmentID) Then
		If not isEmpty(RuleID) and not isNull(RuleID) and IsNumeric(RuleID) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_UpdateProductRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductActionAssignmentID", adInteger, adParamInput,, ProductActionAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)											'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = false
			End If
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateProductRuleAssignment = returnValue
End Function


'************************************************************************************************************************
Function DeleteProductRuleAssignment(ProductID, RuleID)
	Dim returnValue
	returnValue = null

	If not isEmpty(ProductID) and not isNull(ProductID) and IsNumeric(ProductID) Then
		If not isEmpty(RuleID) and not isNull(RuleID) and IsNumeric(RuleID) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_DeleteProductRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)			'2
			call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteProductRuleAssignment = returnValue
End Function
'************************************************************************************************************************
Function DeleteProductRuleAssignmentbyAssignmentID(ProductActionAssignmentID)
	Dim returnValue
	returnValue = null

	If not isEmpty(ProductActionAssignmentID) and not isNull(ProductActionAssignmentID) and IsNumeric(ProductActionAssignmentID) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_DeleteProductRuleAssignmentbyProductActionAssignmentID")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductActionAssignmentID", adInteger, adParamInput,, ProductActionAssignmentID)	'1
			call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductRuleAssignmentbyAssignmentID = returnValue
End Function
'************************************************************************************************************************
'	DESCRIPTION:
'		Get the RuleID for an Product
'		Returns RuleID/Null

Function GetProductRuleAssignment(productID)
	Dim returnValue
	returnValue = null
	If not isEmpty(productID) and not isNull(productID) and IsNumeric(productID) Then
		Dim Cmd
		call OpenSQLConnection(ConnectionType_Attribute)
		set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@productID", adInteger, adParamInput,, productID) 			'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductActionAssignmentID", adInteger, adParamOutput)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamOutput) 							'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(0)		= productID
			ary(1)		= Cmd.Parameters(2).Value
			ary(2)		= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		set Cmd = Nothing
		call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductRuleAssignment = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Get the rules for Products based on the RuleID
'		Returns RuleID/Null

Const trProductRuleProductActionAssignmentID	= 0
Const trProductRuleRuleID						= 1
Const trProductRuleProductID					= 2
Const trProductRuleFriendlyName					= 3
Const trProductRuleProductCode					= 4
Const trProductRuleDateCreated					= 5
Const trProductRuleDateModified					= 6

Function GetTransactiveProductRuleAssignments(RuleID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductRuleAssignmentsByRuleID")
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)		'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetTransactiveProductRuleAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a Product Name from a ProductActionAssignmentID.
'		Returns Product Name/Null

Function GetProductRuleAssignmentName(ProductActionAssignmentID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductActionAssignmentID)) and Not(IsNull(ProductActionAssignmentID)) and Len(ProductActionAssignmentID) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetProductRuleAssignmentName")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductActionAssignmentID", adInteger, adParamInput,, ProductActionAssignmentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput, 75)												'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductRuleAssignmentName = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Check if the Product has a transactive rule assignment
'		True/False

Function HasProductRuleAssignment(ProductID, RuleID)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(ProductID) and Not IsNull(ProductID) and IsNumeric(ProductID) Then
		If Not IsEmpty(RuleID) and Not IsNull(RuleID) and IsNumeric(RuleID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_HasProductRuleAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
				cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)				'2
				cmd.Parameters.Append Cmd.CreateParameter("@HasProductRuleAssignment", adBoolean, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasProductRuleAssignment = ReturnValue
		End If
	End If
End Function


'************************************************************************************************************************
'	DESCRIPTION:
'		Add a manufacture rule assignment.
'		Array/Null

Function AddManufactureRuleAssignment(ManufactureID, RuleID)
	Dim returnValue, Cmd
	returnValue = null
	If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
		If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_AddManufactureRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)						'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureRuleActionAssignmentID", adInteger, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddManufactureRuleAssignment = returnValue
End Function


'************************************************************************************************************************
' Update a rule for a manufacture. 

Function UpdateManufactureRuleAssignment(ManufactureRuleActionAssignmentID, RuleID)
	Dim returnValue
	returnValue = true
	If not isEmpty(ManufactureRuleActionAssignmentID) and not isNull(ManufactureRuleActionAssignmentID) and IsNumeric(ManufactureRuleActionAssignmentID) Then
		If not isEmpty(RuleID) and not isNull(RuleID) and IsNumeric(RuleID) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_UpdateManufactureRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureRuleActionAssignmentID", adInteger, adParamInput,, ManufactureRuleActionAssignmentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)															'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = false
			End If
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateManufactureRuleAssignment = returnValue
End Function


'************************************************************************************************************************
Function DeleteManufactureRuleAssignment(ManufactureRuleActionAssignmentID)
	Dim returnValue
	returnValue = null
	If not isEmpty(ManufactureRuleActionAssignmentID) and not isNull(ManufactureRuleActionAssignmentID) and IsNumeric(ManufactureRuleActionAssignmentID) Then
		Dim Cmd
		call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_DeleteManufactureRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureRuleActionAssignmentID", adInteger, adParamInput,, ManufactureRuleActionAssignmentID)	'1
		call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
		set Cmd = Nothing
		call CloseSQLConnection(ctReadWrite)
	End If
	DeleteManufactureRuleAssignment = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Get the RuleID for a manufacture
'		Returns RuleID/Null

Function GetManufactureRuleAssignment(manufactureID)
	Dim returnValue
	returnValue = null
	If not isEmpty(manufactureID) and not isNull(manufactureID) and IsNumeric(manufactureID) Then
		Dim Cmd
		call OpenSQLConnection(ConnectionType_Attribute)
		set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetManufactureRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@manufactureID", adInteger, adParamInput,, manufactureID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureRuleActionAssignmentID", adInteger, adParamOutput) 	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamOutput) 								'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(0)		= manufactureID
			ary(1)		= Cmd.Parameters(2).Value
			ary(2)		= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		set Cmd = Nothing
		call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetManufactureRuleAssignment = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Get the rules for Manufacturers based on the RuleID
'		Returns RuleID/Null

Const trManufacturerRuleManufactureRuleActionAssignmentID	= 0
Const trManufacturerRuleRuleID								= 1
Const trManufacturerRuleManufacturerID						= 2
Const trManufacturerRuleManufacturerCode					= 3
Const trManufacturerRuleManufacturerName					= 4
Const trManufacturerRuleDateCreated							= 5
Const trManufacturerRuleDateModified						= 6

Function GetTransactiveManufacturerRuleAssignments(RuleID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetManufacturerRuleAssignmentsByRuleID")
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)		'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetTransactiveManufacturerRuleAssignments = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a Product Name from a ManufactureRuleActionAssignmentID.
'		Returns Product Name/Null

Function GetManufacturerRuleAssignmentName(ManufactureRuleActionAssignmentID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ManufactureRuleActionAssignmentID)) and Not(IsNull(ManufactureRuleActionAssignmentID)) and Len(ManufactureRuleActionAssignmentID) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetManufacturerRuleAssignmentName")
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureRuleActionAssignmentID", adInteger, adParamInput,, ManufactureRuleActionAssignmentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarchar, adParamOutput, 75)																'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetManufacturerRuleAssignmentName = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Check if the Manufacture has a transactive rule assignment
'		True/False

Function HasManufactureRuleAssignment(ManufactureID, RuleID)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(ManufactureID) and Not IsNull(ManufactureID) and IsNumeric(ManufactureID) Then
		If Not IsEmpty(RuleID) and Not IsNull(RuleID) and IsNumeric(RuleID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_HasManufactureRuleAssignment")
				cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)					'2
				cmd.Parameters.Append Cmd.CreateParameter("@HasManufactureRuleAssignment", adBoolean, adParamOutput)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasManufactureRuleAssignment = ReturnValue
		End If
	End If
End Function

'************************************************************************************************************************
'	Return a list of all the rules that can be applied to a product, folder, family or manufacture

Function getTransactiveRules()
	Dim returnValue
	returnValue = null
	Dim Cmd
	Dim resultSet
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetTransactiveRules")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If not resultSet.EOF Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	getTransactiveRules = returnValue
End Function


'************************************************************************************************************************
'	DESCRIPTION:
'		Get the list of folders and totals for products
'		Array/Null
'		par = folder list

	const parFolderID	= 0		'FolderID
	const parFolderName	= 1 	'Name
	const parCnetProds	= 2 	'CNet Products In Folder
	const parTotalProds	= 3 	'Total Products In Folder
	const parPercentage	= 4		'Percentage of products approved by CNet

Function GetTransactiveProductApprovedReport()
	Dim returnValue, resultSet
	returnValue = Null
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_GetProductApprovedReport")
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If Not resultSet.EOF Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	GetTransactiveProductApprovedReport = returnValue
End Function


'*******************************************************************************************************
'	Reporting functions
'*******************************************************************************************************
const aeRpt_SummaryReport_UserName = 0
const aeRpt_SummaryReport_EDCCount = 1
const aeRpt_SummaryReport_AttributeCount = 2


'*******************************************************************************************************
' SummaryReport
' AuditActionType - what action to report on? See aeAuditAction_XXXX consts for acceptable values
'*******************************************************************************************************
Function SummaryReport(AuditActionType, StartDate, EndDate)
	Dim returnValue
	Dim Cmd
	Dim resultSet
	Call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute,"WebDB.dbo.Attribute_ReportEntrySummary")
	Cmd.Parameters.Append Cmd.CreateParameter("@ActionID", adInteger, adParamInput,, AuditActionType)	'1
	Cmd.Parameters.Append Cmd.CreateParameter("@StartDate", adDBTimeStamp, adParamInput,, StartDate)	'2
	Cmd.Parameters.Append Cmd.CreateParameter("@EndDate", adDBTimeStamp, adParamInput,, EndDate)		'3
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If not resultSet.EOF Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Attribute)
	SummaryReport = returnValue
End Function


'*******************************************************************************************************
Function GetTotalProductCount()

	Dim returnValue
	returnValue = true
	Dim Cmd
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalProductCount")
	Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)
	Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
	If Cmd.Parameters(0).Value = 0 Then
		returnValue = Cmd.Parameters(1).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetTotalProductCount = returnValue
End Function

'*******************************************************************************************************
' Used when constructing OPTION lists in forms, I got sick of writing this code over and over again

Function shouldbeSelected(s1, s2)
	Dim result

	If CStr(s1)=CStr(s2) Then
		result = "SELECTED"
	else
		result = ""
	End If

	shouldBeSelected = result
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all attributes for the current product.
'		Array/Null

Const aeProductAttributeAttributeValueProductAssignmentID	= 0
Const aeProductAttributeAttributeID							= 1
Const aeProductAttributeAttributeKey						= 2
Const aeProductAttributeAttributeCode						= 3
Const aeProductAttributeName								= 4
Const aeProductAttributeDescription							= 5
Const aeProductAttributeAttributeTypeID						= 6
Const aeProductAttributeAttributeTypeKey					= 7
Const aeProductAttributeAttributeTypeName					= 8
Const aeProductAttributeAttributeTypeDescription			= 9
Const aeProductAttributeCollectionID						= 10
Const aeProductAttributeCollectionKey						= 11
Const aeProductAttributeCollectionName						= 12
Const aeProductAttributeCollectionDescription				= 13
Const aeProductAttributeProductID							= 14
Const aeProductAttributeAttributeValueID					= 15
Const aeProductAttributeAttributeValueKey					= 16
Const aeProductAttributeAttributeValueCode					= 17
Const aeProductAttributeAttributeValueValue					= 18
Const aeProductAttributeAttributeValueSearchableValue		= 19
Const aeProductAttributeAttributeValueNumericValue			= 20
Const aeProductAttributeAttributeValueUnitID				= 21
Const aeProductAttributeUnitKey								= 22
Const aeProductAttributeUnitCode							= 23
Const aeProductAttributeUnitName							= 24
Const aeProductAttributeUnitDescription						= 25
Const aeProductAttributeUnitCollectionID					= 26
Const aeProductAttributeAttributeValueActive				= 27
Const aeProductAttributeAttributeStatusID					= 28
Const aeProductAttributeAttributeStatusKey					= 29
Const aeProductAttributeAttributeStatusName					= 30
Const aeProductAttributeAttributeStatusDescription			= 31
Const aeProductAttributeAttributeFolderID					= 32
Const aeProductAttributeAttributeFolderKey					= 33
Const aeProductAttributeAttributeFolderCode					= 34
Const aeProductAttributeAttributeFolderSortOptionID			= 35
Const aeProductAttributeAttributeFolderDirectoryID			= 36
Const aeProductAttributeAttributeFolderName					= 37
Const aeProductAttributeAttributeFolderDescription			= 38
Const aeProductAttributeUserCreated							= 39
Const aeProductAttributeUserModified						= 40
Const aeProductAttributeDateCreated							= 41
Const aeProductAttributeDateModified						= 42
Const aeProductAttributeKeyAttribute						= 43

Function GetProductAttributes(ProductID, CollectionID, DirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
				Call OpenSQLConnection(ConnectionType_Attribute)
				Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributes")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)	'3
				Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
				If(resultSet.State = adstateOpen) Then
					If(Not(resultSet.EOF)) Then
						returnValue = resultSet.GetRows
					End If
					resultSet.Close
				End If
				Set resultSet = Nothing
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Attribute)
			End If
		End If
	End If
	GetProductAttributes = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all attributes for the current product.
'		Optimized for CDW.com & CDWG.com Product Pages.
'		Array/Null

Const aeProductAttributeByStatusName								= 0
Const aeProductAttributeByStatusAttributeValueValue					= 1
Const aeProductAttributeByStatusUnitName							= 2
Const aeProductAttributeByStatusAttributeFolderName					= 3
Const aeProductAttributeByStatusAttributeSetNo						= 4

Function GetProductAttributesByStatus(ProductID, CollectionID, DirectoryID, AttributeStatusID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
				If(Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
					Call OpenSQLConnection(ConnectionType_Attribute)
					Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributesByStatus")
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
					Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)				'2
					Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)				'3
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'4
					Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
					If(resultSet.State = adstateOpen) Then
						If(Not(resultSet.EOF)) Then
							returnValue = resultSet.GetRows
						End If
						resultSet.Close
					End If
					Set resultSet = Nothing
					Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Attribute)
				End If
			End If
		End If
	End If
	GetProductAttributesByStatus = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns attributes for multiple products.
'		Pass a 1x array, 2x array, or recordset with a column
'		index for the later two specifing ProductCodes's.  Pass a
'		string when you wish to build the where clause yourself.
'		Returns Array/Null

Const aeProductAttributesBulkAttributeID				= 0
Const aeProductAttributesBulkFolderName					= 1
Const aeProductAttributesBulkAttributeName				= 2

' Order by constants
Const aeProductAttributeOrderByAttributeName	= "pa.AttributeName"
Const aeProductAttributeOrderByFolderName		= "pa.FolderName, pa.AttributeName"

' If you make changes to this you will need to duplicate them in the GetProductAttributesBulkByStatus function below...
Function GetProductAttributesBulk(ProductCodes, CollectionID, DirectoryID, ProductCodeIndex, ProductOrderBy)
	Dim returnValue, resultSet, TestDimension, ProductCodeList, i
	returnValue = Null
	If(Not(IsEmpty(ProductCodes)) and Not(IsNull(ProductCodes))) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
				If(Not(IsEmpty(ProductCodeIndex)) and (IsNull(ProductCodeIndex) or IsNumeric(ProductCodeIndex))) Then
					If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If
					If(IsArray(ProductCodes)) Then
						If(Not(Is2xArray(ProductCodes))) Then
							' 1x Array
							ProductCodeList = Join(ProductCodes, ",")
							ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
						Else
							' 2x Array
							ProductCodeList = ""
							For i = Lbound(ProductCodes, 2) To Ubound(ProductCodes, 2)
								If(i = Ubound(ProductCodes, 2)) Then
									ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i)
								Else
									ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i) & ","
								End If
							Next
							ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
						End If
					ElseIf(IsObject(ProductCodes)) Then
						' RecordSet
						If(ProductCodes.State = adstateOpen) Then
							ProductCodes.MoveFirst
							ProductCodeList = ""
							While(Not(ProductCodes.EOF))
								ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex) & ","
								ProductCodes.MoveNext
							Wend
							ProductCodeList = Left(ProductCodeList, Len(ProductCodeList) - 1)
							ProductCodes.MoveFirst
							ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
						End If
					ElseIf(VarType(ProductCodes) = vbString) Then
						' String
						ProductCodeList = ProductCodes
					End If
					If(Not(IsEmpty(ProductCodeList)) and Not(IsNull(ProductCodeList)) and (Len(ProductCodeList) > 0)) Then
						Dim Cmd
						Call OpenSQLConnection(ConnectionType_Attribute)
						Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributesBulk")
						Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'1
						Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)			'2
						Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)				'3
						Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, ProductCodeList)	'4
						Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)	'5
						Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
						If(resultSet.State = adstateOpen) Then
							If(Not(resultSet.EOF)) Then
								returnValue = resultSet.GetRows
							End If
							resultSet.Close
						End If
						Set resultSet = Nothing
						Set Cmd = Nothing
						Call CloseSQLConnection(ConnectionType_Attribute)
					End If
				End If
			End If
		End If
	End If
	GetProductAttributesBulk = returnValue
End Function

'***************************************************************

' Constants defined above

Function GetProductAttributesBulkByStatus(ProductCodes, CollectionID, DirectoryID, ProductCodeIndex, ProductOrderBy, AttributeStatusID)
	Dim returnValue, resultSet, TestDimension, ProductCodeList, i
	returnValue = Null
	If(Not(IsEmpty(ProductCodes)) and Not(IsNull(ProductCodes))) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
				If(Not(IsEmpty(ProductCodeIndex)) and (IsNull(ProductCodeIndex) or IsNumeric(ProductCodeIndex))) Then
					If(Not(IsEmpty(AttributeStatusID)) and (IsNull(AttributeStatusID) or IsNumeric(AttributeStatusID))) Then
						If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If

						If(IsArray(ProductCodes)) Then
							If(Not(Is2xArray(ProductCodes))) Then
								' 1x Array
								ProductCodeList = Join(ProductCodes, ",")
								ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
							Else
								' 2x Array
								ProductCodeList = ""
								For i = Lbound(ProductCodes, 2) To Ubound(ProductCodes, 2)
									If(i = Ubound(ProductCodes, 2)) Then
										ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i)
									Else
										ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i) & ","
									End If
								Next
								ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
							End If
						ElseIf(IsObject(ProductCodes)) Then
							' RecordSet
							If(ProductCodes.State = adstateOpen) Then
								ProductCodes.MoveFirst
								ProductCodeList = ""
								While(Not(ProductCodes.EOF))
									ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex) & ","
									ProductCodes.MoveNext
								Wend
								ProductCodeList = Left(ProductCodeList, Len(ProductCodeList) - 1)
								ProductCodes.MoveFirst
								ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
							End If
						ElseIf(VarType(ProductCodes) = vbString) Then
							' String
							ProductCodeList = ProductCodes
						End If

						If(Not(IsEmpty(ProductCodeList)) and Not(IsNull(ProductCodeList)) and (Len(ProductCodeList) > 0)) Then
							Dim Cmd
							Call OpenSQLConnection(ConnectionType_Attribute)
							Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributesBulkByStatus")
							Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)				'1
							Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)				'2
							Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)					'3
							Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, ProductCodeList)		'4
							Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)		'5
							Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'6
							Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
							If(resultSet.State = adstateOpen) Then
								If(Not(resultSet.EOF)) Then
									returnValue = resultSet.GetRows
								End If
								resultSet.Close
							End If
							Set resultSet = Nothing
							Set Cmd = Nothing
							Call CloseSQLConnection(ConnectionType_Attribute)
						End If
					End If
				End If
			End If
		End If
	End If
	GetProductAttributesBulkByStatus = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns attribute values for multiple products.
'		Pass a 1x array, 2x array, or recordset with a column
'		index for the later two specifing ProductCodes's.  Pass a
'		string when you wish to build the where clause yourself.
'		Returns Array/Null

Const aeProductAttributeValuesBulkAttributeID				= 0
Const aeProductAttributeValuesBulkProductCode				= 1
Const aeProductAttributeValuesBulkAttributeValue			= 2
Const aeProductAttributeValuesBulkUnitName					= 3

' Order by constants
Const aeProductAttributeValueOrderByAttributeName	= "pa.AttributeName, p.ProductCode, pa.AttributeValue"
Const aeProductAttributeValueOrderByProductCode		= "p.ProductCode"

' If you make changes to this you will need to duplicate them in the GetProductAttributeValuesBulkByStatus function below...
Function GetProductAttributeValuesBulk(ProductCodes, CollectionID, ProductCodeIndex, ProductOrderBy)
	Dim returnValue, resultSet, TestDimension, ProductCodeList, i
	returnValue = Null
	If(Not(IsEmpty(ProductCodes)) and Not(IsNull(ProductCodes))) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(ProductCodeIndex)) and (IsNull(ProductCodeIndex) or IsNumeric(ProductCodeIndex))) Then
				If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If
				If(IsArray(ProductCodes)) Then
					If(Not(Is2xArray(ProductCodes))) Then
						' 1x Array
						ProductCodeList = Join(ProductCodes, ",")
						ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
					Else
						' 2x Array
						ProductCodeList = ""
						For i = Lbound(ProductCodes, 2) To Ubound(ProductCodes, 2)
							If(i = Ubound(ProductCodes, 2)) Then
								ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i)
							Else
								ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i) & ","
							End If
						Next
						ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
					End If
				ElseIf(IsObject(ProductCodes)) Then
					' RecordSet
					If(ProductCodes.State = adstateOpen) Then
						ProductCodes.MoveFirst
						ProductCodeList = ""
						While(Not(ProductCodes.EOF))
							ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex) & ","
							ProductCodes.MoveNext
						Wend
						ProductCodeList = Left(ProductCodeList, Len(ProductCodeList) - 1)
						ProductCodes.MoveFirst
						ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
					End If
				ElseIf(VarType(ProductCodes) = vbString) Then
					' String
					ProductCodeList = ProductCodes
				End If
				If(Not(IsEmpty(ProductCodeList)) and Not(IsNull(ProductCodeList)) and (Len(ProductCodeList) > 0)) Then
					Dim Cmd
					Call OpenSQLConnection(ConnectionType_Attribute)
					Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributeValuesBulk")
					Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'1
					Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)				'2
					Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, ProductCodeList)	'3
					Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)	'4
					Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
					If(resultSet.State = adstateOpen) Then
						If(Not(resultSet.EOF)) Then
							returnValue = resultSet.GetRows
						End If
						resultSet.Close
					End If
					Set resultSet = Nothing
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Attribute)
				End If
			End If
		End If
	End If
	GetProductAttributeValuesBulk = returnValue
End Function

'***************************************************************

' Constants defined above

Function GetProductAttributeValuesBulkByStatus(ProductCodes, CollectionID, ProductCodeIndex, ProductOrderBy, AttributeStatusID)
	Dim returnValue, resultSet, TestDimension, ProductCodeList, i
	returnValue = Null
	If(Not(IsEmpty(ProductCodes)) and Not(IsNull(ProductCodes))) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(ProductCodeIndex)) and (IsNull(ProductCodeIndex) or IsNumeric(ProductCodeIndex))) Then
				If(Not(IsEmpty(AttributeStatusID)) and (IsNull(AttributeStatusID) or IsNumeric(AttributeStatusID))) Then
					If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If
					If(IsArray(ProductCodes)) Then
						If(Not(Is2xArray(ProductCodes))) Then
							' 1x Array
							ProductCodeList = Join(ProductCodes, ",")
							ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
						Else
							' 2x Array
							ProductCodeList = ""
							For i = Lbound(ProductCodes, 2) To Ubound(ProductCodes, 2)
								If(i = Ubound(ProductCodes, 2)) Then
									ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i)
								Else
									ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex, i) & ","
								End If
							Next
							ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
						End If
					ElseIf(IsObject(ProductCodes)) Then
						' RecordSet
						If(ProductCodes.State = adstateOpen) Then
							ProductCodes.MoveFirst
							ProductCodeList = ""
							While(Not(ProductCodes.EOF))
								ProductCodeList = ProductCodeList & ProductCodes(ProductCodeIndex) & ","
								ProductCodes.MoveNext
							Wend
							ProductCodeList = Left(ProductCodeList, Len(ProductCodeList) - 1)
							ProductCodes.MoveFirst
							ProductCodeList = "p.ProductCode IN(" & ProductCodeList & ")"
						End If
					ElseIf(VarType(ProductCodes) = vbString) Then
						' String
						ProductCodeList = ProductCodes
					End If

					If(Not(IsEmpty(ProductCodeList)) and Not(IsNull(ProductCodeList)) and (Len(ProductCodeList) > 0)) Then
						Dim Cmd
						Call OpenSQLConnection(ConnectionType_Attribute)
						Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributeValuesBulkByStatus")
						Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)				'1
						Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)					'2
						Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, ProductCodeList)		'3
						Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)		'4
						Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'5
						Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
						If(resultSet.State = adstateOpen) Then
							If(Not(resultSet.EOF)) Then
								returnValue = resultSet.GetRows
							End If
							resultSet.Close
						End If
						Set resultSet = Nothing
						Set Cmd = Nothing
						Call CloseSQLConnection(ConnectionType_Attribute)
					End If
				End If
			End If
		End If
	End If
	GetProductAttributeValuesBulkByStatus = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a single product attribute value.
'		Returns Value/Null

Function GetProductAttributeValue(ProductID, AttributeID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributeValue")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput, 4000)							'3
			Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetProductAttributeValue = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a single product attributes attribute values.
'		Array/Null

Const aeProductAttributeValueAttributeValueProductAssignmentID	= 0
Const aeProductAttributeValueAttributeID						= 1
Const aeProductAttributeValueAttributeKey						= 2
Const aeProductAttributeValueAttributeCode						= 3
Const aeProductAttributeValueName								= 4
Const aeProductAttributeValueDescription						= 5
Const aeProductAttributeValueAttributeTypeID					= 6
Const aeProductAttributeValueAttributeTypeKey					= 7
Const aeProductAttributeValueAttributeTypeName					= 8
Const aeProductAttributeValueAttributeTypeDescription			= 9
Const aeProductAttributeValueCollectionID						= 10
Const aeProductAttributeValueCollectionKey						= 11
Const aeProductAttributeValueCollectionName						= 12
Const aeProductAttributeValueCollectionDescription				= 13
Const aeProductAttributeValueProductID							= 14
Const aeProductAttributeValueAttributeValueID					= 15
Const aeProductAttributeValueAttributeValueKey					= 16
Const aeProductAttributeValueAttributeValueCode					= 17
Const aeProductAttributeValueAttributeValueValue				= 18
Const aeProductAttributeValueAttributeValueSearchableValue		= 19
Const aeProductAttributeValueAttributeValueNumericValue			= 20
Const aeProductAttributeValueAttributeValueUnitID				= 21
Const aeProductAttributeValueUnitKey							= 22
Const aeProductAttributeValueUnitCode							= 23
Const aeProductAttributeValueUnitName							= 24
Const aeProductAttributeValueUnitDescription					= 25
Const aeProductAttributeValueUnitCollectionID					= 26
Const aeProductAttributeValueAttributeValueActive				= 27
Const aeProductAttributeValueAttributeStatusID					= 28
Const aeProductAttributeValueAttributeStatusKey					= 29
Const aeProductAttributeValueAttributeStatusName				= 30
Const aeProductAttributeValueAttributeStatusDescription			= 31
Const aeProductAttributeValueUserCreated						= 32
Const aeProductAttributeValueUserModified						= 33
Const aeProductAttributeValueDateCreated						= 34
Const aeProductAttributeValueDateModified						= 35

Function GetProductAttributeValues(ProductID, AttributeID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributeValues")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)					'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetProductAttributeValues = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes all product attributes values for current product.
'		Error True/False

Function DeleteProductAttributeValues(ProductID, AttributeID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteProductAttributeValues")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteProductAttributeValues = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the attribute template for the current product.
'		Array/Null

Const aeProductAttributeTemplateAttributeID							= 0
Const aeProductAttributeTemplateAttributeKey						= 1
Const aeProductAttributeTemplateAttributeCode						= 2
Const aeProductAttributeTemplateAttributeName						= 3
Const aeProductAttributeTemplateAttributeDescription				= 4
Const aeProductAttributeTemplateAttributeTypeID						= 5
Const aeProductAttributeTemplateAttributeTypeKey					= 6
Const aeProductAttributeTemplateAttributeTypeName					= 7
Const aeProductAttributeTemplateAttributeTypeDescription			= 8
Const aeProductAttributeTemplateCollectionID						= 9
Const aeProductAttributeTemplateCollectionKey						= 10
Const aeProductAttributeTemplateCollectionName						= 11
Const aeProductAttributeTemplateCollectionDescription				= 12
Const aeProductAttributeTemplateFolderID							= 13
Const aeProductAttributeTemplateFolderKey							= 14
Const aeProductAttributeTemplateFolderCode							= 15
Const aeProductAttributeTemplateFolderSortOptionID					= 16
Const aeProductAttributeTemplateFolderDirectoryID					= 17
Const aeProductAttributeTemplateFolderName							= 18
Const aeProductAttributeTemplateFolderDescription					= 19
Const aeProductAttributeTemplateRequired							= 20
Const aeProductAttributeTemplateComparable							= 21
Const aeProductAttributeTemplateSearchable							= 22
Const aeProductAttributeTemplateKeyAttribute						= 23

Function GetProductAttributeTemplate(ProductID, CollectionID, TemplateDirectoryID, CatagoryDirectoryID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			If(Not(IsEmpty(TemplateDirectoryID)) and Not(IsNull(TemplateDirectoryID)) and IsNumeric(TemplateDirectoryID)) Then
				If(Not(IsEmpty(CatagoryDirectoryID)) and Not(IsNull(CatagoryDirectoryID)) and IsNumeric(CatagoryDirectoryID)) Then
					Call OpenSQLConnection(ConnectionType_Attribute)
					Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributeTemplate")
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'1
					Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)					'2
					Cmd.Parameters.Append Cmd.CreateParameter("@TemplateDirectoryID", adInteger, adParamInput,, TemplateDirectoryID)	'3
					Cmd.Parameters.Append Cmd.CreateParameter("@CatagoryDirectoryID", adInteger, adParamInput,, CatagoryDirectoryID)	'4
					Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
					If(resultSet.State = adstateOpen) Then
						If(Not(resultSet.EOF)) Then
							returnValue = resultSet.GetRows
						End If
						resultSet.Close
					End If
					Set resultSet = Nothing
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Attribute)
				End If
			End If
		End If
	End If
	GetProductAttributeTemplate = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates the Attribute status for a products Attribute.
'		Error True/False

Function UpdateProductAttributeStatus(AttributeValueProductAssignmentID, AttributeStatusID)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(AttributeValueProductAssignmentID)) and Not(IsNull(AttributeValueProductAssignmentID)) and Len(AttributeValueProductAssignmentID) > 0 Then
			If Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and Len(AttributeStatusID) > 0 Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateProductAttributeStatus")
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeValueProductAssignmentID", adInteger, adParamInput,, AttributeValueProductAssignmentID)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)									'2
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If Cmd.Parameters(0).Value = 0 Then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	UpdateProductAttributeStatus = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Product Content																										*
'*			Wrappers for Product Content																					*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds Product Content.
'		Returns ProductContentID/Null

Function AddProductContent(ProductID, ContentTypeID, Value, CollectionID, AttributeStatusID)
	Dim returnValue, Cmd
	returnValue = Null
	If (Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If (Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
			If (Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0 )Then
				If (Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
					If (Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
						Call OpenSQLConnection(ctReadWrite)
						Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddProductContent")
						Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'1
						Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)				'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamInput,8000, Value)							'3
						Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)					'4
						Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)		'5
						Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamOutput)							'6
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If(Cmd.Parameters(0).Value = 0) Then
							returnValue = Cmd.Parameters(6).Value
						End If
						Set Cmd = Nothing
						Call CloseSQLConnection(ctReadWrite)
					End If
				End If
			End If
		End If
	End If
	AddProductContent = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Creates Product Content user information.
'		Error True/False

Function SetProductContentUserCreated(ProductContentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetProductContentUserCreated")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamInput,, ProductContentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetProductContentUserCreated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates Product Content user information.
'		Error True/False

Function SetProductContentUserModified(ProductContentID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_SetProductContentUserModified")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamInput,, ProductContentID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamInput, 50, UserName)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	SetProductContentUserModified = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes ProductContent.
'		Error True/False

Function DeleteProductContent(ProductContentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductContentID)) and Not(IsNull(ProductContentID)) and IsNumeric(ProductContentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteProductContent")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamInput,, ProductContentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductContent = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates Product Content.
'		Error True/False

Function UpdateProductContent(ProductContentID, ProductID, ContentTypeID, Value, CollectionID, AttributeStatusID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductContentID)) and Not(IsNull(ProductContentID)) and IsNumeric(ProductContentID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
				If(Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0 ) Then
					If (Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
						If (Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
							Call OpenSQLConnection(ctReadWrite)
							Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateProductContent")
							Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamInput,, ProductContentID)			'1
							Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'3
							Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)				'4
							Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamInput,8000, Value)							'5
							Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)					'6
							Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)		'7
							Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
							If(Cmd.Parameters(0).Value = 0) Then
								returnValue = False
							End If
							Set Cmd = Nothing
							Call CloseSQLConnection(ctReadWrite)
						End If
					End If
				End If
			End If
		End If
	End If
	UpdateProductContent = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns productcontent based on ProductContentID.
'		Array/Null

Const aeProductContentID							= 0
Const aeProductContentProductID						= 1
Const aeProductContentTypeID						= 2
Const aeProductContentTypeCode						= 3
Const aeProductContentTypeKey						= 4
Const aeProductContentTypeName						= 5
Const aeProductContentTypeDescription				= 6
Const aeProductContentValue							= 7
Const aeProductContentCollectionID					= 8
Const aeProductContentCollectionKey					= 9
Const aeProductContentCollectionName				= 10
Const aeProductContentCollectionDescription			= 11
Const aeProductContentAttributeStatusID				= 12
Const aeProductContentAttributeStatusKey			= 13
Const aeProductContentAttributeStatusName			= 14
Const aeProductContentAttributeStatusDescription	= 15
Const aeProductContentSearchableValue				= 16
Const aeProductContentUserModified					= 17
Const aeProductContentUserCreated					= 18
Const aeProductContentDateCreated					= 19
Const aeProductContentDateModified					= 20

Function GetProductContent(ProductContentID)
	Dim returnValue, ary(20), Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductContentID)) and Not(IsNull(ProductContentID)) and IsNumeric(ProductContentID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContent")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamInput,, ProductContentID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adinteger, adParamOutput)							'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamOutput)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeCode", adVarChar, adParamOutput,15)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeKey", adChar, adParamOutput, 32)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'6
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'7
		Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput,8000)							'8
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)						'9
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamOutput, 32)						'10
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionName", adVarChar, adParamOutput, 75)					'11
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionDescription", adVarChar, adParamOutput, 500)			'12
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamOutput)					'13
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusKey", adChar, adParamOutput, 32)					'14
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusName", adVarChar, adParamOutput, 75)				'15
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusDescription", adVarChar, adParamOutput, 500)		'16
		Cmd.Parameters.Append Cmd.CreateParameter("@SearchableValue", adVarChar, adParamOutput, 255)				'17
		Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)					'18
		Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)						'19
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)						'20
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)					'21
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeProductContentID)							= ProductContentID
			ary(aeProductContentProductID)					= Cmd.Parameters(2).Value
			ary(aeProductContentTypeID)						= Cmd.Parameters(3).Value
			ary(aeProductContentTypeCode)					= Cmd.Parameters(4).Value
			ary(aeProductContentTypeKey)					= Cmd.Parameters(5).Value
			ary(aeProductContentTypeName)					= Cmd.Parameters(6).Value
			ary(aeProductContentTypeDescription)			= Cmd.Parameters(7).Value
			ary(aeProductContentValue)						= Cmd.Parameters(8).Value
			ary(aeProductContentCollectionID)				= Cmd.Parameters(9).Value
			ary(aeProductContentCollectionKey)				= Cmd.Parameters(10).Value
			ary(aeProductContentCollectionName)				= Cmd.Parameters(11).Value
			ary(aeProductContentCollectionDescription)		= Cmd.Parameters(12).Value
			ary(aeProductContentAttributeStatusID)			= Cmd.Parameters(13).Value
			ary(aeProductContentAttributeStatusKey)			= Cmd.Parameters(14).Value
			ary(aeProductContentAttributeStatusName)		= Cmd.Parameters(15).Value
			ary(aeProductContentAttributeStatusDescription)	= Cmd.Parameters(16).Value
			ary(aeProductContentSearchableValue)			= Cmd.Parameters(17).Value
			ary(aeProductContentUserModified)				= Cmd.Parameters(18).Value
			ary(aeProductContentUserCreated)				= Cmd.Parameters(19).Value
			ary(aeProductContentDateCreated)				= Cmd.Parameters(20).Value
			ary(aeProductContentDateModified)				= Cmd.Parameters(21).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductContent = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all content for the current product.
'		Array/Null

Const aeProductIDProductContentID			= 0
Const aeProductIDContentTypeID				= 1
Const aeProductIDContentTypeCode			= 2
Const aeProductIDContentTypeKey				= 3
Const aeProductIDContentTypeName			= 4
Const aeProductIDContentTypeDescription		= 5
Const aeProductIDValue						= 6
Const aeProductIDCollectionID				= 7
Const aeProductIDCollectionKey				= 8
Const aeProductIDCollectionName				= 9
Const aeProductIDCollectionDescription		= 10
Const aeProductIDAttributeStatusID			= 11
Const aeProductIDAttributeStatusKey			= 12
Const aeProductIDAttributeStatusName		= 13
Const aeProductIDAttributeStatusDescription	= 14
Const aeProductIDSearchableValue			= 15
Const aeProductIDUserModified				= 16
Const aeProductIDUserCreated				= 17
Const aeProductIDDateCreated				= 18
Const aeProductIDDateModified				= 19

Function GetProductContentByProduct(ProductID, CollectionID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentByProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End IF
	GetProductContentByProduct = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns productcontent based on ProductID and content type.
'		Array/Null

Const aeContentTypeIDProductContentTypeID			= 0
Const aeContentTypeIDProductID						= 1
Const aeContentTypeIDCollectionID					= 2
Const aeContentTypeIDProductContentID				= 3
Const aeContentTypeIDProductContentTypeCode			= 4
Const aeContentTypeIDProductContentTypeKey			= 5
Const aeContentTypeIDProductContentTypeName			= 6
Const aeContentTypeIDProductContentTypeDescription	= 7
Const aeContentTypeIDProductContentValue			= 8
Const aeContentTypeIDCollectionKey					= 9
Const aeContentTypeIDCollectionName					= 10
Const aeContentTypeIDCollectionDescription			= 11
Const aeContentTypeIDAttributeStatusID				= 12
Const aeContentTypeIDAttributeStatusKey				= 13
Const aeContentTypeIDAttributeStatusName			= 14
Const aeContentTypeIDAttributeStatusDescription		= 15
Const aeContentTypeIDProductContentSearchableValue	= 16
Const aeContentTypeIDProductContentUserModified		= 17
Const aeContentTypeIDProductContentUserCreated		= 18
Const aeContentTypeIDProductContentDateCreated		= 19
Const aeContentTypeIDProductContentDateModified		= 20

Function GetProductContentByType(ContentTypeID, ProductID, CollectionID)
	Dim returnValue, ary(22), Cmd
	returnValue = Null
	If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentByType")
			Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)		'3
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamOutput)				'4
			Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeCode", adVarChar, adParamOutput,15)				'5
			Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeKey", adChar, adParamOutput, 32)					'6
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)						'7
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)				'8
			Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput,8000)						'9
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamOutput, 32)					'10
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionName", adVarChar, adParamOutput, 75)				'11
			Cmd.Parameters.Append Cmd.CreateParameter("@CollectionDescription", adVarChar, adParamOutput, 500)		'12
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamOutput)				'13
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusKey", adChar, adParamOutput, 32)				'14
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusName", adVarChar, adParamOutput, 75)			'15
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusDescription", adVarChar, adParamOutput, 500)	'16
			Cmd.Parameters.Append Cmd.CreateParameter("@SearchableValue", adVarChar, adParamOutput, 255)			'17
			Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)				'18
			Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)					'19
			Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)					'10
			Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)				'21
			Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				ary(aeContentTypeIDProductContentTypeID)			= ContentTypeID
				ary(aeContentTypeIDProductID)						= ProductID
				ary(aeContentTypeIDCollectionID)					= CollectionID
				ary(aeContentTypeIDProductContentID)				= Cmd.Parameters(4).Value
				ary(aeContentTypeIDProductContentTypeCode)			= Cmd.Parameters(5).Value
				ary(aeContentTypeIDProductContentTypeKey)			= Cmd.Parameters(6).Value
				ary(aeContentTypeIDProductContentTypeName)			= Cmd.Parameters(7).Value
				ary(aeContentTypeIDProductContentTypeDescription)	= Cmd.Parameters(8).Value
				ary(aeContentTypeIDProductContentValue)				= Cmd.Parameters(9).Value
				ary(aeContentTypeIDCollectionKey)					= Cmd.Parameters(10).Value
				ary(aeContentTypeIDCollectionName)					= Cmd.Parameters(11).Value
				ary(aeContentTypeIDCollectionDescription)			= Cmd.Parameters(12).Value
				ary(aeContentTypeIDAttributeStatusID)				= Cmd.Parameters(13).Value
				ary(aeContentTypeIDAttributeStatusKey)				= Cmd.Parameters(14).Value
				ary(aeContentTypeIDAttributeStatusName)				= Cmd.Parameters(15).Value
				ary(aeContentTypeIDAttributeStatusDescription)		= Cmd.Parameters(16).Value
				ary(aeContentTypeIDProductContentSearchableValue)	= Cmd.Parameters(17).Value
				ary(aeContentTypeIDProductContentUserModified)		= Cmd.Parameters(18).Value
				ary(aeContentTypeIDProductContentUserCreated)		= Cmd.Parameters(19).Value
				ary(aeContentTypeIDProductContentDateCreated)		= Cmd.Parameters(20).Value
				ary(aeContentTypeIDProductContentDateModified)		= Cmd.Parameters(21).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End IF
	End If
	GetProductContentByType = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a single product content value.
'		Returns Value/Null

Function GetProductContentValue(ProductID, ContentTypeID, CollectionID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
			If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
				Call OpenSQLConnection(ConnectionType_Attribute)
				Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentValue")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'3
				Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput, 4000)							'4
				Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Attribute)
			End If
		End If
	End If
	GetProductContentValue = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a single product content value by status.
'		Returns Value/Null

Function GetProductContentValueByStatus(ProductID, ContentTypeID, CollectionID, AttributeStatusID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
			If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
				If(Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID)) Then
					Call OpenSQLConnection(ConnectionType_Attribute)
					Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentValueByStatus")
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)		'2
					Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)			'3
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)'4
					Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput, 4000)							'5
					Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
					If(Cmd.Parameters(0).Value = 0) Then
						returnValue = Cmd.Parameters(5).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ConnectionType_Attribute)
				End If
			End If
		End If
	End If
	GetProductContentValueByStatus = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns productcontent based on ProductID, contenttype and version ID.
'		Array/Null

Const aeContentHistoryTypeProductContentTypeID			= 0
Const aeContentHistoryTypeProductID						= 1
Const aeContentHistoryTypeVersionID						= 2
Const aeContentHistoryTypeProductContentHistoryID		= 3
Const aeContentHistoryTypeProductContentID				= 4
Const aeContentHistoryTypeProductContentTypeCode		= 5
Const aeContentHistoryTypeProductContentTypeKey			= 6
Const aeContentHistoryTypeProductContentTypeName		= 7
Const aeContentHistoryTypeProductContentTypeDescription	= 8
Const aeContentHistoryTypeProductContentValue			= 9
Const aeContentHistoryTypeCollectionID					= 10
Const aeContentHistoryTypeCollectionKey					= 11
Const aeContentHistoryTypeCollectionName				= 12
Const aeContentHistoryTypeCollectionDescription			= 13
Const aeContentHistoryTypeAttributeStatusID				= 14
Const aeContentHistoryTypeAttributeStatusKey			= 15
Const aeContentHistoryTypeAttributeStatusName			= 16
Const aeContentHistoryTypeAttributeStatusDescription	= 17
Const aeContentHistoryTypeProductContentSearchableValue	= 18
Const aeContentHistoryTypeProductContentUserModified	= 19
Const aeContentHistoryTypeProductContentUserCreated		= 20
Const aeContentHistoryTypeProductContentDateCreated		= 21
Const aeContentHistoryTypeProductContentDateModified	= 22

Function GetProductContentHistoryByType(ProductID, ContentTypeID, VersionID)
	Dim returnValue, ary(22), Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
			If(Not(IsEmpty(VersionID)) and Not(IsNull(VersionID)) and IsNumeric(VersionID)) Then
				Call OpenSQLConnection(ConnectionType_Attribute)
				Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentHistoryByType")
				Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'2
				Cmd.Parameters.Append Cmd.CreateParameter("@VersionID", adInteger, adParamInput,, VersionID)			'3
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentHistoryID", adInteger, adParamOutput)			'4
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamOutput)				'5
				Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeCode", adVarChar, adParamOutput,15)				'6
				Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeKey", adChar, adParamOutput, 32)					'7
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)						'8
				Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)				'9
				Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput,8000)						'10
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamOutput)					'11
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionKey", adChar, adParamOutput, 32)					'12
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionName", adVarChar, adParamOutput, 75)				'13
				Cmd.Parameters.Append Cmd.CreateParameter("@CollectionDescription", adVarChar, adParamOutput, 500)		'14
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamOutput)				'15
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusKey", adChar, adParamOutput, 32)				'16
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusName", adVarChar, adParamOutput, 75)			'17
				Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusDescription", adVarChar, adParamOutput, 500)	'18
				Cmd.Parameters.Append Cmd.CreateParameter("@SearchableValue", adVarChar, adParamOutput, 255)			'19
				Cmd.Parameters.Append Cmd.CreateParameter("@UserModified", adVarChar, adParamOutput, 50)				'20
				Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamOutput, 50)					'21
				Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)					'22
				Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)				'23
				Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					ary(aeContentHistoryTypeProductContentTypeID)			= ContentTypeID
					ary(aeContentHistoryTypeProductID)						= ProductID
					ary(aeContentHistoryTypeVersionID)						= VersionID
					ary(aeContentHistoryTypeProductContentHistoryID)		= Cmd.Parameters(4).Value
					ary(aeContentHistoryTypeProductContentID)				= Cmd.Parameters(5).Value
					ary(aeContentHistoryTypeProductContentTypeCode)			= Cmd.Parameters(6).Value
					ary(aeContentHistoryTypeProductContentTypeKey)			= Cmd.Parameters(7).Value
					ary(aeContentHistoryTypeProductContentTypeName)			= Cmd.Parameters(8).Value
					ary(aeContentHistoryTypeProductContentTypeDescription)	= Cmd.Parameters(9).Value
					ary(aeContentHistoryTypeProductContentValue)			= Cmd.Parameters(10).Value
					ary(aeContentHistoryTypeCollectionID)					= Cmd.Parameters(11).Value
					ary(aeContentHistoryTypeCollectionKey)					= Cmd.Parameters(12).Value
					ary(aeContentHistoryTypeCollectionName)					= Cmd.Parameters(13).Value
					ary(aeContentHistoryTypeCollectionDescription)			= Cmd.Parameters(14).Value
					ary(aeContentHistoryTypeAttributeStatusID)				= Cmd.Parameters(15).Value
					ary(aeContentHistoryTypeAttributeStatusKey)				= Cmd.Parameters(16).Value
					ary(aeContentHistoryTypeAttributeStatusName)			= Cmd.Parameters(17).Value
					ary(aeContentHistoryTypeAttributeStatusDescription)		= Cmd.Parameters(18).Value
					ary(aeContentHistoryTypeProductContentSearchableValue)	= Cmd.Parameters(19).Value
					ary(aeContentHistoryTypeProductContentUserModified)		= Cmd.Parameters(20).Value
					ary(aeContentHistoryTypeProductContentUserCreated)		= Cmd.Parameters(21).Value
					ary(aeContentHistoryTypeProductContentDateCreated)		= Cmd.Parameters(22).Value
					ary(aeContentHistoryTypeProductContentDateModified)		= Cmd.Parameters(23).Value
					returnValue = ary
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Attribute)
			End If
		End If
	End If
	GetProductContentHistoryByType = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a single product content history value.
'		Returns Value/Null

Function GetProductContentHistoryValue(ProductID, ContentTypeID, VersionID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
			If(Not(IsEmpty(VersionID)) and Not(IsNull(VersionID)) and IsNumeric(VersionID)) Then
				Call OpenSQLConnection(ConnectionType_Attribute)
				Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductContentHistoryValue")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@VersionID", adInteger, adParamInput,, VersionID)				'3
				Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamOutput, 4000)							'4
				Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Attribute)
			End If
		End If
	End If
	GetProductContentHistoryValue = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product has a specific content piece.
'		Returns True/False

Function ProductHasProductContent(ContentTypeID, ProductID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(ContentTypeID) and Not IsNull(ContentTypeID) and IsNumeric(ContentTypeID) Then
		If Not IsEmpty(ProductID) and Not IsNull(ProductID) and IsNumeric(ProductID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_ProductHasProductContent")
			Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Exists", adBoolean, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	ProductHasProductContent = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		update a Product content status.
'		Error True/False

Function UpdateProductContentStatus(ProductContentID, AttributeStatusID)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and Len(AttributeStatusID) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateProductContentStatus")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductContentID", adInteger, adParamInput,, ProductContentID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'2

			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	UpdateProductContentStatus = returnValue
End Function


'****************************************************************************************************************************
'*		Section Description:																								*
'*		Product Content	Types																								*
'*			Wrappers for Product Content Types																				*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds a Content type.
'		Returns ContentTypeID/Null

Function AddContentType(Name, Description)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddContentType")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddContentType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a Content type.
'		Error True/False

Function DeleteContentType(ContentTypeID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteContentType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteContentType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a Content type.
'		Error True/False

Function UpdateContentType(ContentTypeID, Name, Description)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateContentType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)		'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateContentType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an Content type.
'		Array/Null

Const aeContentTypeID			= 0
Const aeContentTypeKey			= 1
Const aeContentTypeName			= 2
Const aeContentTypeDescription	= 3
Const aeContentTypeDateCreated	= 4
Const aeContentTypeDateModified	= 5

Function GetContentType(ContentTypeID)
	Dim returnValue, ary(5), Cmd
	returnValue = Null
	If(Not(IsEmpty(ContentTypeID)) and Not(IsNull(ContentTypeID)) and IsNumeric(ContentTypeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetContentType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)							'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)					'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)						'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)					'6
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeContentTypeID)			= ContentTypeID
			ary(aeContentTypeKey)			= Cmd.Parameters(2).Value
			ary(aeContentTypeName)			= Cmd.Parameters(3).Value
			ary(aeContentTypeDescription)	= Cmd.Parameters(4).Value
			ary(aeContentTypeDateCreated)	= Cmd.Parameters(5).Value
			ary(aeContentTypeDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetContentType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an ContentTypeID from the guid.
'		Returns ContentTypeID/Null

Function GetContentTypeID(ContentTypeKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ContentTypeKey)) and Not(IsNull(ContentTypeKey)) and Len(ContentTypeKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetContentTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeKey", adChar, adParamInput, 32, ContentTypeKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetContentTypeID = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Add/Check Folders, create folder linking hierarchy, update Product_ProductFolderAssignment.
Function AddCheckUnspscCodeRecursive(ProductID, UNSPSC)
	Dim returnValue
	returnValue = null
	
	If Not IsEmpty(ProductID) And Not IsNull(ProductID) And IsNumeric(ProductID) And Len(UNSPSC) = 8 Then
		Dim Cmd
		call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_AddCheckUnspscCodeRecursive")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UNSPSC", adVarChar, adParamInput,8, UNSPSC)			'2
		call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
		set Cmd = Nothing
		call CloseSQLConnection(ctReadWrite)
	End If
	AddCheckUnspscCodeRecursive = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		update a Product content status.
'		Error True/False

Function UpdateProductStatus(ProductID, AttributeStatusID, UserName)
	Dim returnValue
	returnValue = True
		If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) Then
			If Not(IsEmpty(AttributeStatusID)) and Not(IsNull(AttributeStatusID)) and IsNumeric(AttributeStatusID) Then
				If Not(IsEmpty(UserName)) and Not(IsNull(UserName)) and Len(UserName) > 0  Then
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateProductStatus")
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)	'2
					Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarchar, adParamInput,50, UserName)					'3
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 Then
						returnValue = False
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	UpdateProductStatus = returnValue
End Function
Const ae_AttributeProductStatusInProgressCount	= 1
Const ae_AttributeProductStatusCompleteCount	= 2
Const ae_AttributeProductStatusRejectedCount	= 3
Const ae_AttributeProductStatusApprovedCount	= 4

'***************************************************************
'	DESCRIPTION:
'		Gets the Attribute Status summary for a Product
'		Returns Product Status/NULL

Function GetStatusCountForProduct(productID)
	Dim returnValue
	returnValue = null
	If not isEmpty(productID) and not isNull(productID) and IsNumeric(productID) Then
		Dim Cmd
		call OpenSQLConnection(ConnectionType_Attribute)
		set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetStatusCountForProduct")
		Cmd.Parameters.Append Cmd.CreateParameter("@productID", adInteger, adParamInput,, productID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@InProgress", adInteger, adParamOutput)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Complete", adInteger, adParamOutput)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Rejected", adInteger, adParamOutput)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Approved", adInteger, adParamOutput)				'5
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)

		If Cmd.Parameters(0).Value = 0 Then
			Dim ary(4)
			ary(ae_AttributeProductStatusInProgressCount) = Cmd.Parameters(2).Value
			ary(ae_AttributeProductStatusCompleteCount) = Cmd.Parameters(3).Value
			ary(ae_AttributeProductStatusRejectedCount)= Cmd.Parameters(4).Value
			ary(ae_AttributeProductStatusApprovedCount)	= Cmd.Parameters(5).Value
			returnValue = ary
		End If
		set Cmd = Nothing
		call CloseSQLConnection(ConnectionType_Attribute)

	End If
	GetStatusCountForProduct = returnValue
End Function
'****************************************************************************************************************************
'*		Section Description:																								*
'*		CNet Reports																										*
'*			Wrappers for CNet Reports																						*
'****************************************************************************************************************************


'***************************************************************
'	DESCRIPTION:
'		Returns all the users that have effected active products
'		attribute counts.
'		Array/Null

Const aeReportTouchedAttributeUserName			= 0
Const aeReportTouchedAttributeCount				= 1

Function ReportTouchedAttributes(CollectionID, CompanyID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTouchedAttributes")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTouchedAttributes = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all the users that have effected active products
'		content counts.
'		Array/Null

Const aeReportTouchedContentUserName			= 0
Const aeReportTouchedContentCount				= 1

Function ReportTouchedContent(CollectionID, CompanyID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTouchedContent")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTouchedContent = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the number of assigned attributes.
'		Returns Count/Null

Function ReportTotalAttributes(CollectionID, CompanyID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalAttributes")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTotalAttributes = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the number of assigned content.
'		Returns Count/Null

Function ReportTotalContent(CollectionID, CompanyID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalContent")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTotalContent = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the number of active products assigned attributes.
'		Returns Count/Null

Function ReportTotalProductAttributes(CollectionID, CompanyID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalProductAttributes")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTotalProductAttributes = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the number of active products assigned content.
'		Returns Count/Null

Function ReportTotalProductContent(CollectionID, CompanyID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalProductContent")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTotalProductContent = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the number of active products
'		Returns Count/Null

Function ReportTotalProduct()
	Dim returnValue, Cmd
	returnValue = Null
	Call OpenSQLConnection(ConnectionType_Attribute)
	Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'1
	Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
	If(Cmd.Parameters(0).Value = 0) Then
		returnValue = Cmd.Parameters(1).Value
	End If
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Attribute)
	ReportTotalProduct = returnValue
End Function

'*******************************************************************************************************
Function GetTotalManufacturerCount(CompanyID)

	Dim returnValue
	returnValue = true
	Dim Cmd
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalManufacturers")
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'2
	Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
	If Cmd.Parameters(0).Value = 0 Then
		returnValue = Cmd.Parameters(2).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetTotalManufacturerCount = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns the number of active products with Photo Captions.
'		Returns Count/Null

Function ReportTotalPhotoCaptions(CollectionID, CompanyID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(CollectionID)) and Not(IsNull(CollectionID)) and IsNumeric(CollectionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_ReportTotalPhotoCaptions")
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	ReportTotalPhotoCaptions = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all the Active Skus from the CNet Tables
'		Returns Array/Null

const asrProdID				= 0
const asrStatusCode			= 1
const asrtimestamp			= 2
const asrProductName		= 3
const asrManufacturerID		= 4
const asrManufacturerName	= 5

Function GetCNetActiveSkusReport()
	Dim returnValue
	returnValue = null
	Dim Cmd
	Dim resultSet
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "CDS..CNet_GetCNETActiveSkusReport")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If not resultSet.EOF Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetCNetActiveSkusReport = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a list of dates and the number of EDCs transferred
'		Returns Array/Null

const ddpTimeStamp			= 0
const ddpEDCCount			= 1

Function ReportDailyDataPump()
	Dim returnValue
	returnValue = null
	Dim Cmd
	Dim resultSet
	call OpenSQLConnection(ConnectionType_Attribute)
	set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "CDS..CNet_ReportDailyDataPump")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If not resultSet.EOF Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	ReportDailyDataPump = returnValue
End Function
'****************************************************************************************************************************
'*		Section Description:																								*
'*		ReportTypes																											*
'*			Wrappers for ReportTypes 																						*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds a ReportType.
'		Returns ReportTypeID/Null

Function AddReportType(Name, Description)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddReportType")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 255, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddReportType = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a ReportType.
'		Error True/False

Function DeleteReportType(ReportTypeID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ReportTypeID)) and Not(IsNull(ReportTypeID)) and IsNumeric(ReportTypeID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteReportType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamInput,, ReportTypeID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteReportType = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a ReportType.
'		Error True/False

Function UpdateReportType(ReportTypeID, Name, Description)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateReportType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamInput,, ReportTypeID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateReportType = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns an ReportType.
'		Array/Null

Const aeReportTypeID			= 0
Const aeReportTypeKey			= 1
Const aeReportTypeName			= 2
Const aeReportTypeDescription	= 3
Const aeReportTypeDateCreated	= 4
Const aeReportTypeDateModified	= 5

Function GetReportType(ReportTypeID)
	Dim returnValue, ary(5), Cmd
	returnValue = Null
	If(Not(IsEmpty(ReportTypeID)) and Not(IsNull(ReportTypeID)) and IsNumeric(ReportTypeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetReportType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamInput,, ReportTypeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeKey", adChar, adParamOutput, 32)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'6
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeReportTypeID)				= ReportTypeID
			ary(aeReportTypeKey)			= Cmd.Parameters(2).Value
			ary(aeReportTypeName)			= Cmd.Parameters(3).Value
			ary(aeReportTypeDescription)	= Cmd.Parameters(4).Value
			ary(aeReportTypeDateCreated)	= Cmd.Parameters(5).Value
			ary(aeReportTypeDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetReportType = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a ReportTypeID from the guid.
'		Returns ReportTypeID/Null

Function GetReportTypeID(ReportTypeKey)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ReportTypeKey)) and Not(IsNull(ReportTypeKey)) and Len(ReportTypeKey) > 0) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetReportTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeKey", adChar, adParamInput, 32, ReportTypeKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetReportTypeID = returnValue
End Function
'****************************************************************************************************************************
'*		Section Description:																								*
'*		ReportValues																										*
'*			Wrappers for ReportValues 																						*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds a ReportValue.
'		Returns ReportValueID/Null

Function AddReportValue(ReportTypeID, Name, Value, CompanyID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ReportTypeID)) and Not(IsNull(ReportTypeID)) and IsNumeric(ReportTypeID) ) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(IsEmpty(Value)) and Not(IsNull(Value)) and Len(Value) > 0) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddReportValue")
				Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamInput,, ReportTypeID)		'1
				Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 50, Name)					'2
				Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarChar, adParamInput, 100, Value)				'3
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)			'4
				Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeID", adInteger, adParamOutput)					'5
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(5).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddReportValue = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Returns all the Report Values (based on ReportTypeKey for simplicity)
'		Returns Array/Null

const arvReportValueID				= 0
const arvReportTypeID				= 1
const arvName						= 2
const arvValue						= 3
const arvDateCreated				= 4
const arvCompanyID					= 5

Function GetReportValues(ReportTypeKey, CompanyID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ReportTypeKey)) and Not(IsNull(ReportTypeKey)) and Len(ReportTypeKey) = 32 ) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetReportValues")
		Cmd.Parameters.Append Cmd.CreateParameter("@ReportTypeKey", AdChar, adParamInput,32, ReportTypeKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetReportValues = returnValue
End Function
'****************************************************************************************************************************
'*		Section Description:																								*
'*		Product Attribute Folder Sort Assignments																			*
'*			Wrappers for roduct Attribute Folder Sort Assignments															*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an attribute sort assignment to a product folder.
'		Returns SortAssignmentID/Null

Function AddProductAttributeFolderSortAssignment(ProductFolderID, AttributeFolderID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductFolderID)) and Not(IsNull(ProductFolderID)) and IsNumeric(ProductFolderID)) Then
		If(Not(IsEmpty(AttributeFolderID)) and Not(IsNull(AttributeFolderID)) and IsNumeric(AttributeFolderID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddProductAttributeFolderSortAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderID", adInteger, adParamInput,, ProductFolderID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderID", adInteger, adParamInput,, AttributeFolderID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@SortAssignmentID", adInteger, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductAttributeFolderSortAssignment = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Deletes an attribute sort assignment from a product folder.
'		Error True/False

Function DeleteProductAttributeFolderSortAssignment(SortAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(SortAssignmentID)) and Not(IsNull(SortAssignmentID)) and IsNumeric(SortAssignmentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteProductAttributeFolderSortAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@SortAssignmentID", adInteger, adParamInput,, SortAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductAttributeFolderSortAssignment = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		updates an attribute sort assignment from a product folder.
'		Error True/False

Function UpdateProductAttributeFolderSortAssignment(SortAssignmentID, ProductFolderID, AttributeFolderID, SortRank)
	Dim returnValue
	returnValue = true
	If not isEmpty(SortAssignmentID) and not isNull(SortAssignmentID) and IsNumeric(SortAssignmentID) Then
		If not isEmpty(ProductFolderID) and not isNull(ProductFolderID) and IsNumeric(ProductFolderID) Then
			If not isEmpty(AttributeFolderID) and not isNull(AttributeFolderID) and IsNumeric(AttributeFolderID) Then
				If not isEmpty(SortRank) and not isNull(SortRank) and IsNumeric(SortRank) Then
					Dim Cmd
					call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_UpdateProductAttributeFolderSortAssignment")
					Cmd.Parameters.Append Cmd.CreateParameter("@SortAssignmentID", adInteger, adParamInput,, SortAssignmentID)		'1
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderID", adInteger, adParamInput,, ProductFolderID)		'2					
					Cmd.Parameters.Append Cmd.CreateParameter("@AttributeFolderID", adInteger, adParamInput,, AttributeFolderID)		'3
					Cmd.Parameters.Append Cmd.CreateParameter("@SortRank", adInteger, adParamInput,, SortRank)	'4
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 Then
						returnValue = false
					End If
					set Cmd = Nothing
					call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	UpdateProductAttributeFolderSortAssignment = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns all attribute sort assignments from a product folder.
'		Array/Null

Const aeProductAttributeFolderSortAssignmentID					= 0
Const aeProductAttributeFolderSortAssignmentProductFolderID		= 1
Const aeProductAttributeFolderSortAssignmentProductFolderName	= 2
Const aeProductAttributeFolderSortAssignmentProductFolderDesc	= 3
Const aeProductAttributeFolderSortAssignmentAttributeFolderID	= 4
Const aeProductAttributeFolderSortAssignmentAttributeFolderName	= 5
Const aeProductAttributeFolderSortAssignmentAttributeFolderDesc	= 6
Const aeProductAttributeFolderSortAssignmentSortRank			= 7
Const aeProductAttributeFolderSortAssignmentDateCreated			= 8
Const aeProductAttributeFolderSortAssignmentDateModified		= 9

Function GetProductAttributeFolderSortAssignments(ProductFolderID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductFolderID)) and Not(IsNull(ProductFolderID)) and IsNumeric(ProductFolderID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetProductAttributeFolderSortAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderID", adInteger, adParamInput,, ProductFolderID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetProductAttributeFolderSortAssignments = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns the highest sort rank for a productfolderID.
'		Array/Null

Function GetLatestProductAttributeForlderSortRank(ProductFolderID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductFolderID)) and Not(IsNull(ProductFolderID)) and IsNumeric(ProductFolderID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetLatestProductAttributeForlderSortRank")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderID", adInteger, adParamInput,, ProductFolderID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@SortRank", adInteger, adParamOutput)							'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetLatestProductAttributeForlderSortRank = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all CNet data feed request information.
'		Array/Null

Const aeCNetDataFeedRequestFileExtensionID	= 0
Const aeCNetDataFeedRequestFileExtension	= 1
Const aeCNetDataFeedRequestSuccess			= 2
Const aeCNetDataFeedRequestDateCreated		= 3
Const aeCNetDataFeedRequestRegionID		    = 4
Const aeCNetDataFeedRequestStatus		    = 5
Function GetCNetDataFeedRequestFileExtensions()
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	call OpenSQLConnection(ConnectionType_Attribute)
	Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetCNetDataFeedRequestFileExtensions")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetCNetDataFeedRequestFileExtensions = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the array for a product record requested from CNet.
'		Returns Array/Null

Const aeProductDataFeedRequestProductID		= 0
Const aeProductDataFeedRequestRequestID		= 1
Const aeProductDataFeedRequestAction		= 2
Const aeProductDataFeedRequestSent			= 3
Const aeProductDataFeedRequestDateSent		= 4
Const aeProductDataFeedRequestDateCreated	= 5

Function GetCNetProductDataFeedRequest(ProductID)
	Dim returnValue, ary(5), Cmd
	returnValue = Null
	If Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and Len(ProductID) > 0 Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetCNetProductDataFeedRequest")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@RequestID", adInteger, adParamOutput)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Action", adChar, adParamOutput, 1)						'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Sent", adBoolean, adParamOutput)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSent", adDBTimeStamp, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'6
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(aeProductDataFeedRequestProductID)		= ProductID
			ary(aeProductDataFeedRequestRequestID)		= Cmd.Parameters(2).Value
			ary(aeProductDataFeedRequestAction)			= Cmd.Parameters(3).Value
			ary(aeProductDataFeedRequestSent)			= Cmd.Parameters(4).Value
			ary(aeProductDataFeedRequestDateSent)		= Cmd.Parameters(5).Value
			ary(aeProductDataFeedRequestDateCreated)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetCNetProductDataFeedRequest = returnValue
End Function

'*******************************************************************************************************
Function GetTotalProductCountByRuleID(RuleID)
	Dim returnValue, Cmd
	returnValue = true
	If Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID) Then
		call OpenSQLConnection(ConnectionType_Attribute)
		set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetTotalProductCountByRuleID")
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Count", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(2).Value
		End If
		set Cmd = Nothing
		call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetTotalProductCountByRuleID = returnValue
End Function

'****************************************************************************************************************************
'*		Section Description:																								*
'*		Folder Sort Assignments																								*
'*			Wrappers for Folder Sort Assignments																			*
'****************************************************************************************************************************
'	DESCRIPTION:
'		Adds an sort assignment to a product folder.
'		Returns FolderSortAssignmentID/Null

Function AddFolderSortAssignment(FolderID, SortedFolderID, FolderType)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(SortedFolderID)) and Not(IsNull(SortedFolderID)) and IsNumeric(SortedFolderID)) Then
			If(Not(IsEmpty(FolderType)) and Not(IsNull(FolderType)) and IsNumeric(FolderType)) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_AddFolderSortAssignment")
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@SortedFolderID", adInteger, adParamInput,, SortedFolderID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderType", adSmallInt, adParamInput,, FolderType)			'3
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderSortAssignmentID", adInteger, adParamOutput)			'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(4).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddFolderSortAssignment = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Deletes sort assignment from a product folder.
'		Error True/False

Function DeleteFolderSortAssignment(FolderSortAssignmentID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(FolderSortAssignmentID)) and Not(IsNull(FolderSortAssignmentID)) and IsNumeric(FolderSortAssignmentID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_DeleteFolderSortAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderSortAssignmentID", adInteger, adParamInput,, FolderSortAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteFolderSortAssignment = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		updates an attribute sort assignment from a product folder.
'		Error True/False

Function UpdateFolderSortAssignment(FolderSortAssignmentID, SortOrder)
	Dim returnValue
	returnValue = true
	If not isEmpty(FolderSortAssignmentID) and not isNull(FolderSortAssignmentID) and IsNumeric(FolderSortAssignmentID) Then
	'	If not isEmpty(FolderID) and not isNull(FolderID) and IsNumeric(FolderID) Then
	'		If not isEmpty(SortedFolderID) and not isNull(SortedFolderID) and IsNumeric(SortedFolderID) Then
	'			If not isEmpty(FolderType) and not isNull(FolderType) and IsNumeric(FolderType) Then
					If not isEmpty(SortOrder) and not isNull(SortOrder) and IsNumeric(SortOrder) Then
						Dim Cmd
						call OpenSQLConnection(ctReadWrite)
						set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Directory_UpdateFolderSortAssignment")
						Cmd.Parameters.Append Cmd.CreateParameter("@FolderSortAssignmentID", adInteger, adParamInput,, FolderSortAssignmentID)	'1
	'					Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)								'2
	'					Cmd.Parameters.Append Cmd.CreateParameter("@SortedFolderID", adInteger, adParamInput,, SortedFolderID)					'3
	'					Cmd.Parameters.Append Cmd.CreateParameter("@FolderType", adSmallInt, adParamInput,, FolderType)							'4
						Cmd.Parameters.Append Cmd.CreateParameter("@SortOrder", adInteger, adParamInput,, SortOrder)							'5
						Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
						If Cmd.Parameters(0).Value = 0 Then
							returnValue = false
						End If
						set Cmd = Nothing
						call CloseSQLConnection(ctReadWrite)
					End If
	'			End If
	'		End If
	'	End If
	End If
	UpdateFolderSortAssignment = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns all sort assignments from a product folder and foldertype (attribute/accessories).
'		Array/Null

Const aeFolderSortAssignmentsFolderSortAssignmentID	= 0
Const aeFolderSortAssignmentsFolderID				= 1
Const aeFolderSortAssignmentsFolderName				= 2
Const aeFolderSortAssignmentsFolderDesc				= 3
Const aeFolderSortAssignmentsSortedFolderID			= 4
Const aeFolderSortAssignmentsSortedFolderName		= 5
Const aeFolderSortAssignmentsSortedFolderDesc		= 6
Const aeFolderSortAssignmentsFolderType				= 7
Const aeFolderSortAssignmentsSortOrder				= 8
Const aeFolderSortAssignmentsDateCreated			= 9
Const aeFolderSortAssignmentsDateModified			= 10

Function GetFolderSortAssignments(FolderID, FolderType)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Directory_GetFolderSortAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderType", adSmallInt, adParamInput,, FolderType)	'2
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetFolderSortAssignments = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns the highest sort rank for a folderID.
'		Array/Null

Function GetLatestFolderSortOrder(FolderID, FolderType)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(FolderType)) and Not(IsNull(FolderType)) and IsNumeric(FolderType)) Then
			Call OpenSQLConnection(ConnectionType_Attribute)
			Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Directory_GetLatestFolderSortOrder")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderType", adSmallInt, adParamInput,, FolderType)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@SortOrder", adInteger, adParamOutput)					'3
			Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Attribute)
		End If
	End If
	GetLatestProductAttributeForlderSortRank = returnValue
End Function

'*********************************************************************************************************
'	Function encapsulates business logic for copying products in bulk, this needed to be fast. Bulk copied
'	products are inProgress by default.

' 	Checks whether to copy Accessories, Attributes and Content.  
'*********************************************************************************************************
Function BulkProductCopy(EDC1, EDC2, Accessories, Attributes, Content, UserName, AttributeStatusID, CollectionID)
	Dim returnValue, Status, Cmd
	returnValue = 0
	call OpenSQLConnection(ctReadWrite)

	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkProductCopy")
		Cmd.Parameters.Append Cmd.CreateParameter("@EDC1", adVarchar, adParamInput, 15, EDC1)
		Cmd.Parameters.Append Cmd.CreateParameter("@EDC2", adVarchar, adParamInput, 15, EDC2)
		Cmd.Parameters.Append Cmd.CreateParameter("@Accessories", adBoolean, adParamInput,, Accessories)
		Cmd.Parameters.Append Cmd.CreateParameter("@Attributes", adBoolean, adParamInput,, Attributes)
		Cmd.Parameters.Append Cmd.CreateParameter("@Content", adBoolean, adParamInput,, Content)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarchar, adParamInput, 50, UserName)
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	BulkProductCopy = returnValue
End Function

'*********************************************************************************************************
'	Function encapsulates business logic for adding product photos in bulk, this needed to be fast. Bulk added 
'	photos are complete by default.

' 	Smart enough to update if the product photo is already present.
'*********************************************************************************************************
Function BulkAddPhotoAssignments(EDC1, EDC2, CollectionID, ContentTypeID, AttributeStatusID, UserName)
	Dim returnValue, Status, Cmd
	returnValue = 0
	call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkPhotoAssignments")
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC1", adVarchar, adParamInput, 15, EDC1)
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC2", adVarchar, adParamInput, 15, EDC2)
	Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)
	Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)
	Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)
	Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarchar, adParamInput, 50, UserName)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	BulkAddPhotoAssignments = returnValue
End Function

Function BulkAddProductContentAssignments(ProductID1, EDC2, CollectionID, ContentTypeID, ContentValue, AttributeStatusID, UserName, UpdateStatement)
	Dim returnValue, Status, Cmd
	returnValue = 0

	call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkProductContentAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID1", adInteger, adParamInput,, ProductID1)
		Cmd.Parameters.Append Cmd.CreateParameter("@EDC2", adVarchar, adParamInput, 15, EDC2)
		Cmd.Parameters.Append Cmd.CreateParameter("@CollectionID", adInteger, adParamInput,, CollectionID)
		Cmd.Parameters.Append Cmd.CreateParameter("@ContentTypeID", adInteger, adParamInput,, ContentTypeID)
		Cmd.Parameters.Append Cmd.CreateParameter("@Value", adVarchar, adParamInput,5000, ContentValue)
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeStatusID", adInteger, adParamInput,, AttributeStatusID)
		Cmd.Parameters.Append Cmd.CreateParameter("@UserName", adVarchar, adParamInput, 50, UserName)
		Cmd.Parameters.Append Cmd.CreateParameter("@UpdateStatment", adVarchar, adParamInput, 75, UpdateStatement)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If

	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	BulkAddProductContentAssignments = returnValue
End Function

' 	Smart enough not to add duplicates If the pair is already present.
'*********************************************************************************************************
Function Attribute_BulkAccessoryDelete(EDC1, EDC2)
	Dim returnValue, Status, Cmd
	returnValue = 0
	call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkAccessoryDelete")
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC1", adVarchar, adParamInput, 15, EDC1)
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC2", adVarchar, adParamInput, 15, EDC2)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	Attribute_BulkAccessoryDelete = returnValue
End Function

' 	Smart enough not to add duplicates If the pair is already present.
'*********************************************************************************************************
Function Attribute_BulkAccessoryDeleteByParentEDC(EDC)
	Dim returnValue, Status, Cmd
	returnValue = 0
	call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkAccessoryDeleteByParentEDC")
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC", adVarchar, adParamInput, 15, EDC)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	Attribute_BulkAccessoryDeleteByParentEDC = returnValue
End Function

'*********************************************************************************************************
Function Attribute_BulkAccessoryDeleteByChildEDC(EDC)
	Dim returnValue, Status, Cmd
	returnValue = 0
	call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkAccessoryDeleteByChildEDC")
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC", adVarchar, adParamInput, 15, EDC)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	Attribute_BulkAccessoryDeleteByChildEDC = returnValue
End Function

'*********************************************************************************************************
'	Function encapsulates business logic for adding accessories in bulk, this needed to be fast. Bulk added 
'	accessories are approved by default.

' 	Smart enough not to add duplicates If the pair is already present.
'*********************************************************************************************************
Function Attribute_BulkAddAccessory(EDC1, EDC2, UserCreated)
	Dim returnValue, Status, Cmd
	returnValue = 0
	call OpenSQLConnection(ctReadWrite)
	set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_BulkAccessoryAdd")
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC1", adVarchar, adParamInput, 15, EDC1)
	Cmd.Parameters.Append Cmd.CreateParameter("@EDC2", adVarchar, adParamInput, 15, EDC2)
	Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarchar, adParamInput, 50, UserCreated)
	Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
	If Cmd.Parameters(0).Value <> 0 Then
		returnValue = Cmd.Parameters(0).Value
	End If
	set Cmd = Nothing
	call CloseSQLConnection(ctReadWrite)
	Attribute_BulkAddAccessory = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a product to an employee.
'		Returns EmployeeProductID/Null

Function AddEmployeeProduct(EmployeeID, ProductID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(EmployeeID)) and Not(IsNull(EmployeeID)) and IsNumeric(EmployeeID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_AddEmployeeProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeProductID", adInteger, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddEmployeeProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a attribute type.
'		Error True/False

Function DeleteEmployeeProduct(EmployeeProductID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(EmployeeProductID)) and Not(IsNull(EmployeeProductID)) and IsNumeric(EmployeeProductID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteEmployeeProduct")
		Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeProductID", adInteger, adParamInput,, EmployeeProductID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteEmployeeProduct = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Deletes a product from the employee assignment table.  Will delete more than one row if more than one person has modified the product.
'		Error True/False

Function DeleteEmployeeProductsByProductID(ProductID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_DeleteEmployeeProductsByProductID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteEmployeeProductsByProductID = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Check if the Product Folder has a transactive rule assignment
'		True/False

Function HasEmployeeProduct(EmployeeID, ProductID)
	dim ReturnValue, cmd
	ReturnValue = Null
	If Not IsEmpty(EmployeeID) and Not IsNull(EmployeeID) and IsNumeric(EmployeeID) Then
		If Not IsEmpty(ProductID) and Not IsNull(ProductID) and IsNumeric(ProductID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Attribute_HasEmployeeProduct")
				cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)	'1
				cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'2
				cmd.Parameters.Append Cmd.CreateParameter("@HasEmployeeProduct", adBoolean, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)

			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			End If

			Set Cmd = Nothing
			HasEmployeeProduct = ReturnValue
		End If
	End If
End Function
'***************************************************************
'	DESCRIPTION:
'		Returns all attributeTypes.
'		Array/Null

Const aeEmployeeProductID			= 0
Const aeEmployeeProductEmployeeID	= 1
Const aeEmployeeProductProductID	= 2
Const aeEmployeeProductDateCreated	= 3
Const aeEmployeeProductDateModified	= 4

Function GetEmployeeProducts(EmployeeID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	call OpenSQLConnection(ConnectionType_Attribute)
	Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetEmployeeProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@EmployeeID", adInteger, adParamInput,, EmployeeID)
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetEmployeeProducts = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Check the status of the CNet File
'		Returns File Status (int)
Function GetCNETFileStatus(Extension)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(Extension)) and Not(IsNull(Extension)) and IsNumeric(Extension)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetCNetDataFeedRequestFileStatus")
		Cmd.Parameters.Append Cmd.CreateParameter("@Extension", adChar, adParamInput,3, Extension)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Status", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetCNETFileStatus = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns all the products for a specific file extension.
'		Array/Null

Const cnFileExtensionID	= 0
Const cnProductID		= 1
Const cnAction			= 2
Const cnProductCode		= 3
Const cnFriendlyName	= 4
Const cnFriendlyDesc	= 5
Const cnDateSent		= 6

Function GetCNetDataFeedProductsByFileExtensionID(FileExtensionID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(FileExtensionID)) and Not(IsNull(FileExtensionID)) and IsNumeric(FileExtensionID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetCNetDataFeedProductsByFileExtensionID")
		Cmd.CommandTimeout = 3600
		Cmd.Parameters.Append Cmd.CreateParameter("@FileExtensionID", adInteger, adParamInput,, FileExtensionID)	'1
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

	GetCNetDataFeedProductsByFileExtensionID = returnValue
End Function

'****************************************************************************************************************************
'	DESCRIPTION:
'		Returns all the products for a specific file extension.
'		Array/Null

' Constants defined above

Function GetCNetDataFeedProductsByDate(DateSent)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(DateSent)) and Not(IsNull(DateSent)) and Len(DateSent)) >= 8 Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_GetCNetDataFeedProductsByDate")
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSent", adDBTimeStamp, adParamInput,, DateSent)	'1
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

	GetCNetDataFeedProductsByDate = returnValue
End Function
'****************************************************************************************************************************
'	DESCRIPTION:
'		updates an CNet Data Files Status.
'		Error True/False

Function UpdateCNETDataFileExtensionStatus(FileExtensionID, Status)
	Dim returnValue
	returnValue = true
	If not isEmpty(FileExtensionID) and not isNull(FileExtensionID) and IsNumeric(FileExtensionID) Then
		If not isEmpty(Status) and not isNull(Status) and IsNumeric(Status) Then
			Dim Cmd
			call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_UpdateCNetDataFeedFileStatus")
			Cmd.Parameters.Append Cmd.CreateParameter("@FileExtensionID", adInteger, adParamInput,, FileExtensionID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adInteger, adParamInput,, Status)						'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 Then
				returnValue = false
			End If
			set Cmd = Nothing
			call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateCNETDataFileExtensionStatus = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a FileExtension from the FileExtensionID.
'		Returns FileExtension/Null

Function GetCNETDataPumpFileExtension(FileExtensionID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(FileExtensionID)) and Not(IsNull(FileExtensionID)) and IsNumeric(FileExtensionID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetCNETDataPumpFileExtensionByID")
		Cmd.Parameters.Append Cmd.CreateParameter("@FileExtensionID", adInteger, adParamInput,, FileExtensionID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FileExtension", adVarChar, adParamOutput, 3)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetCNETDataPumpFileExtension = returnValue
End Function

Function GetAttributeName(AttributeID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(AttributeID)) and Not(IsNull(AttributeID)) and IsNumeric(AttributeID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetAttributeName")
		Cmd.Parameters.Append Cmd.CreateParameter("@AttributeID", adInteger, adParamInput,, AttributeID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'2
		Call ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetAttributeName = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all the old AE attributes.
'		Array/Null

Const aeLegacyAttributeProductAttributeName		= 0
Const aeLegacyAttributeProductAttributeValue	= 1
Const aeLegacyAttributeProductUnitName			= 2
Const aeLegacyAttributeProductCategory			= 3

Function GetLegacyAttributeProducts(ProductID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	call OpenSQLConnection(ConnectionType_Attribute)
	Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Attribute_GetLegacyAttributeProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.getRows
		End If
		resultSet.Close
	End If
	set resultSet = Nothing
	set Cmd = Nothing
	call CloseSQLConnection(ConnectionType_Attribute)
	GetLegacyAttributeProducts = returnValue
End Function

'************************************************************************************************************************
'	Product Accessory Rules																								 
'************************************************************************************************************************
'	DESCRIPTION:
'		Add a product accessories rule assignment.
'		Array/Null

Function AddProductAccessoriesRuleAssignment(RuleID, ManufacturerID, FolderID, FolderType, UserName)
	Dim returnValue, Cmd
	returnValue = null
		If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_AddProductAccessoryRuleAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerID", adInteger, adParamInput,, ManufacturerID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'3
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderType", adInteger, adParamInput,, FolderType)				'4
			Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamInput,50, UserName)				'5
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryRuleID", adInteger, adParamOutput)				'6
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(6).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	AddProductAccessoriesRuleAssignment = returnValue
End Function

'************************************************************************************************************************
Function DeleteProductAccessoriesRuleAssignment(ProductAccessoryRuleID)
	Dim returnValue
	returnValue = null
	If not isEmpty(ProductAccessoryRuleID) and not isNull(ProductAccessoryRuleID) and IsNumeric(ProductAccessoryRuleID) Then
		Dim Cmd
		call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_DeleteProductAccessoryRuleAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryRuleID", adInteger, adParamInput,, ProductAccessoryRuleID)	'1
		call ExecuteSQLProcedure(ctReadWrite, Cmd, false)
		set Cmd = Nothing
		call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductAccessoriesRuleAssignment = returnValue
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		Get the rules for product accessories based on the RuleID
'		Returns RuleID/Null

Const trAccessoriesRuleRuleAssignmentID	= 0
Const trAccessoriesRuleRuleID			= 1
Const trAccessoriesRuleManufacturerID	= 2
Const trAccessoriesRuleManufacturerCode	= 3
Const trAccessoriesRuleManufacturerName	= 4
Const trAccessoriesRuleFolderID			= 5
Const trAccessoriesRuleFolderCode		= 6
Const trAccessoriesRuleFolderName		= 7
Const trAccessoriesRuleFolderType		= 8
Const trAccessoriesRuleDateCreated		= 9
Const trAccessoriesRuleDateModified		= 10

Function GetTransactiveProductAccessoryRuleAssignments(RuleID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(RuleID)) and Not(IsNull(RuleID)) and IsNumeric(RuleID)) Then
		Call OpenSQLConnection(ConnectionType_Attribute)
		Set Cmd = CreateSQLProcedure(ConnectionType_Attribute, "WebDB.dbo.Transactive_GetTransactiveProductAccessoryRuleAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)		'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Attribute, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Attribute)
	End If
	GetTransactiveProductAccessoryRuleAssignments = returnValue
End Function

'******************************************************************************************************************************
'	DESCRIPTION:
'		Get the GetProductAccessoryRule from the ProductAccessoryRuleID.
'		Array/Null

	Const parAProductAccessoryRuleID	= 0
	Const parARuleID					= 1
	Const parAManufacturerID			= 2
	Const parAManufactureCode			= 3
	Const parAManufactureName			= 4
	Const parAFolderID					= 5
	Const parAFolderCode				= 6
	Const parAFolderName				= 7
	Const parAFolderType				= 8
	Const parAUserCreated				= 9
	Const parADateCreated				= 10
	Const parADateModified				= 11

Function GetProductAccessoryRule(ProductAccessoryRuleID)
	Dim returnValue, cmd
	returnValue = Null
	If Not(IsEmpty(ProductAccessoryRuleID)) and Not(IsNull(ProductAccessoryRuleID)) and isNumeric(ProductAccessoryRuleID) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_GetProductAccessoryRule")
			cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryRuleID", adInteger, adParamInput,, ProductAccessoryRuleID)	'1
			cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamOutput)											'2
			cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)									'3
			cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", advarChar, adParamOutput,32)								'4
			cmd.Parameters.Append Cmd.CreateParameter("@ManufactureName", advarChar, adParamOutput,75)								'5
			cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)										'6
			cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", advarChar, adParamOutput,32)									'7
			cmd.Parameters.Append Cmd.CreateParameter("@FolderName", advarChar, adParamOutput,75)									'8
			cmd.Parameters.Append Cmd.CreateParameter("@FolderType", adInteger, adParamOutput)										'9
			cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", advarChar, adParamOutput,50)									'10
			cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)									'11
			cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)								'12

		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		Call CloseSQLConnection(ctReadWrite)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(11)
			ary(paraProductAccessoryRuleID)	= ProductAccessoryRuleID
			ary(paraRuleID)					= Cmd.Parameters(2).Value
			ary(paraManufacturerID)			= Cmd.Parameters(3).Value
			ary(paraManufactureCode)		= Cmd.Parameters(4).Value
			ary(paraManufactureName)		= Cmd.Parameters(5).Value
			ary(paraFolderID)				= Cmd.Parameters(6).Value
			ary(paraFolderCode)				= Cmd.Parameters(7).Value
			ary(paraFolderName)				= Cmd.Parameters(8).Value
			ary(paraFolderType)				= Cmd.Parameters(9).Value
			ary(paraUserCreated)			= Cmd.Parameters(10).Value
			ary(paraDateCreated)			= Cmd.Parameters(11).Value
			ary(paraDateModified)			= Cmd.Parameters(12).Value
			returnValue = ary
		End If

		Set Cmd = Nothing
		GetProductAccessoryRule = ReturnValue
	End If
End Function
'************************************************************************************************************************
'	DESCRIPTION:
'		Check if the MFG and Folder have a transactive accessory rule assignment
'		True/False

Function HasProductAccessoryRuleAssignment(RuleID, ManufacturerID, FolderID )
	dim ReturnValue, cmd
	ReturnValue = Null

	If Not IsEmpty(RuleID) and Not IsNull(RuleID) and IsNumeric(RuleID) Then
		If Not IsEmpty(ManufacturerID) and Not IsNull(ManufacturerID) and IsNumeric(ManufacturerID) Then
			If Not IsEmpty(ManufacturerID) and Not IsNull(ManufacturerID) and IsNumeric(ManufacturerID) Then
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_HasProductAccessoryRuleAssignment")
					cmd.Parameters.Append Cmd.CreateParameter("@RuleID", adInteger, adParamInput,, RuleID)						'1
					cmd.Parameters.Append Cmd.CreateParameter("@ManufacturerID", adInteger, adParamInput,, ManufacturerID)		'2
					cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'3
					cmd.Parameters.Append Cmd.CreateParameter("@HasProductAccessoryRuleAssignment", adBoolean, adParamOutput)	'4
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				Call CloseSQLConnection(ctReadWrite)

				If Cmd.Parameters(0).Value = 0 then
					ReturnValue = Cmd.Parameters(4).Value
				End If

				Set Cmd = Nothing
				HasProductAccessoryRuleAssignment = ReturnValue
			End If
		End If
	End If
End Function

'************************************************************************************************************************
'	DESCRIPTION:
'		RemoveAccessoryByMfgAndPGM

Function RemoveAccessoryByMfgAndPGM(MfgCode, FolderCode)
	dim ReturnValue, cmd
	ReturnValue = Null

	If Not IsEmpty(MfgCode) and Not IsNull(MfgCode) Then
		If Not IsEmpty(FolderCode) and Not IsNull(FolderCode) Then			
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Transactive_RemoveAccessoryByMfgAndPGM")	
				Cmd.Parameters.Append Cmd.CreateParameter("@MfgCode", adVarChar, adParamInput,10, MfgCode)
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adVarChar, adParamInput,10, FolderCode)		
				cmd.Parameters.Append Cmd.CreateParameter("@RemoveCount", adInteger, adParamOutput)	
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			Call CloseSQLConnection(ctReadWrite)
			If Cmd.Parameters(0).Value = 0 then
				ReturnValue = Cmd.Parameters(3).Value
			Else
				ReturnValue = 0
			End If
				Set Cmd = Nothing
			RemoveAccessoryByMfgAndPGM = ReturnValue
		End If
	End If
End Function
%>