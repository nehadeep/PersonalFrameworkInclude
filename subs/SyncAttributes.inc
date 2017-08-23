<%

dim gl_sync_cmdUpdateAttribute, gl_sync_cmdGetProductIDfromCode, gl_sync_cmdGetAttributeIDfromCode, gl_sync_cmdGetUnitofMeasureIDfromName, gl_sync_cmdRemoveAllAttributesExcept, gl_sync_cmdRemoveAttribute, gl_sync_cmdAddAttribute, gl_sync_salesGetAttribute
dim gl_sync_salesRS, gl_sync_RS
dim  gl_sync_NameID, gl_sync_BodyID, gl_sync_ObjectiveID, gl_sync_SubjectiveID, gl_sync_PhotoFileID, gl_sync_PhotoCaptionID
'dim start

function SyncAttributes(EDCList)
	SyncAttributesEX EDCList, false
end function

function SyncAttributesEx(EDCList, Live)
	dim index
	dim aryEDCS
	dim start
	start = timer


	OpenSQLConnection(ctLegacy)


	set gl_sync_cmdGetProductIDfromCode = CreateObject("ADODB.Command")
	set gl_sync_cmdGetProductIDfromCode = CreateStoredProcCommand("WebDB.dbo.Product_GetProductIDByCode")
	gl_sync_cmdGetProductIDfromCode.Parameters.Append gl_sync_cmdGetProductIDfromCode.CreateParameter("ProductCode", adVarChar, adParamInput, 15, NULL) '1
	gl_sync_cmdGetProductIDfromCode.Parameters.Append gl_sync_cmdGetProductIDfromCode.CreateParameter("ProductID", adInteger, adParamOutput, 4, NULL)	'2
	gl_sync_cmdGetProductIDfromCode.ActiveConnection = OpenSQLConnection(ctReadWrite)

	set gl_sync_cmdGetAttributeIDfromCode = CreateObject("ADODB.Command")
	set gl_sync_cmdGetAttributeIDfromCode = CreateStoredProcCommand("WebDB.dbo.Attribute_GetAttributeIDByCode")
	gl_sync_cmdGetAttributeIDfromCode.Parameters.Append gl_sync_cmdGetAttributeIDfromCode.CreateParameter("AttributeCode", adInteger, adParamInput, 4, NULL)	'1
	gl_sync_cmdGetAttributeIDfromCode.Parameters.Append gl_sync_cmdGetAttributeIDfromCode.CreateParameter("AttributeID", adInteger, adParamOutput, 4, NULL)		'2
	gl_sync_cmdGetAttributeIDfromCode.ActiveConnection = GetSQLConnection(ctReadWrite)

	set gl_sync_cmdGetUnitofMeasureIDfromName = CreateObject("ADODB.Command")
	set gl_sync_cmdGetUnitofMeasureIDfromName = CreateStoredProcCommand("WebDB.dbo.Attribute_GetUnitofMeasureIDByName")
	gl_sync_cmdGetUnitofMeasureIDfromName.Parameters.Append gl_sync_cmdGetUnitofMeasureIDfromName.CreateParameter("Name", advarchar, adParamInput, 75, NULL)	'1
	gl_sync_cmdGetUnitofMeasureIDfromName.Parameters.Append gl_sync_cmdGetUnitofMeasureIDfromName.CreateParameter("UOMID", adInteger, adParamOutput, 4, NULL)		'2
	gl_sync_cmdGetUnitofMeasureIDfromName.ActiveConnection = GetSQLConnection(ctReadWrite)

	if live then
		set gl_sync_cmdRemoveAllAttributesExcept = CreateObject("ADODB.Command")
		set gl_sync_cmdRemoveAllAttributesExcept = CreateStoredProcCommand("WebDB.dbo.Attribute_RemoveAllAttributesExcept")
		gl_sync_cmdRemoveAllAttributesExcept.Parameters.Append gl_sync_cmdRemoveAllAttributesExcept.CreateParameter("ProductID", adInteger, adParamInput, 4, NULL)			'1
		gl_sync_cmdRemoveAllAttributesExcept.Parameters.Append gl_sync_cmdRemoveAllAttributesExcept.CreateParameter("AttributeIDs", adVarchar, adParamInput, 2000, NULL)		'2
		gl_sync_cmdRemoveAllAttributesExcept.Parameters.Append gl_sync_cmdRemoveAllAttributesExcept.CreateParameter("AttributeCodes", adVarchar, adParamInput, 2000, NULL)	'3
		gl_sync_cmdRemoveAllAttributesExcept.ActiveConnection = GetSQLConnection(ctReadWrite)
	else
		set gl_sync_cmdRemoveAllAttributesExcept = CreateObject("ADODB.Command")
		set gl_sync_cmdRemoveAllAttributesExcept = CreateStoredProcCommand("WebDB.dbo.Attribute_RemoveAllAttributesStagingExcept")
		gl_sync_cmdRemoveAllAttributesExcept.Parameters.Append gl_sync_cmdRemoveAllAttributesExcept.CreateParameter("ProductID", adInteger, adParamInput, 4, NULL)			'1
		gl_sync_cmdRemoveAllAttributesExcept.Parameters.Append gl_sync_cmdRemoveAllAttributesExcept.CreateParameter("AttributeIDs", adVarchar, adParamInput, 2000, NULL)		'2
		gl_sync_cmdRemoveAllAttributesExcept.Parameters.Append gl_sync_cmdRemoveAllAttributesExcept.CreateParameter("AttributeCodes", adVarchar, adParamInput, 2000, NULL)	'3
		gl_sync_cmdRemoveAllAttributesExcept.ActiveConnection = GetSQLConnection(ctReadWrite)
	end if

	if live then
		set gl_sync_cmdRemoveAttribute = CreateObject("ADODB.Command")
		set gl_sync_cmdRemoveAttribute = CreateStoredProcCommand("WebDB.dbo.Attribute_RemoveAttribute")
		gl_sync_cmdRemoveAttribute.Parameters.Append gl_sync_cmdRemoveAttribute.CreateParameter("ProductID", adInteger, adParamInput, 4, NULL)			'1
		gl_sync_cmdRemoveAttribute.Parameters.Append gl_sync_cmdRemoveAttribute.CreateParameter("AttributeID", adInteger, adParamInput, 4, NULL)		'2
		gl_sync_cmdRemoveAttribute.ActiveConnection = GetSQLConnection(ctReadWrite)
	else
		set gl_sync_cmdRemoveAttribute = CreateObject("ADODB.Command")
		set gl_sync_cmdRemoveAttribute = CreateStoredProcCommand("WebDB.dbo.Attribute_RemoveAttributesStaging")
		gl_sync_cmdRemoveAttribute.Parameters.Append gl_sync_cmdRemoveAttribute.CreateParameter("ProductID", adInteger, adParamInput, 4, NULL)			'1
		gl_sync_cmdRemoveAttribute.Parameters.Append gl_sync_cmdRemoveAttribute.CreateParameter("AttributeID", adInteger, adParamInput, 4, NULL)		'2
		gl_sync_cmdRemoveAttribute.ActiveConnection = GetSQLConnection(ctReadWrite)
	end if

	set gl_sync_cmdAddAttribute = CreateObject("ADODB.Command")
	set gl_sync_cmdAddAttribute = CreateStoredProcCommand("WebDB.dbo.Attribute_AddAttributeSync")
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("Name", adVarChar, adParamInput, 75, NULL)					'1
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("Description", adVarChar, adParamInput, 500, NULL)			'2
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("Type", adInteger, adParamInput, 4, NULL)						'3
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("DefaultUnitOfMeasureID", adInteger, adParamInput, 4, NULL)	'4
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("Required", adBoolean, adParamInput, 1, 0)					'5
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("Comparable", adBoolean, adParamInput, 1, 0)					'6
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("AttributeCode", adInteger, adParamInput, 4, NULL)			'7
	gl_sync_cmdAddAttribute.Parameters.Append gl_sync_cmdAddAttribute.CreateParameter("AttributeID", adInteger, adParamOutput, 4, NULL)				'8
	gl_sync_cmdAddAttribute.ActiveConnection = GetSQLConnection(ctReadWrite)

	set gl_sync_salesGetAttribute = CreateObject("ADODB.Command")
	set gl_sync_salesGetAttribute = CreateStoredProcCommand("AdvertisementsDB..Attribute_GetAttribute")
	gl_sync_salesGetAttribute.Parameters.Append gl_sync_salesGetAttribute.CreateParameter("AttributeID", adInteger, adParamInput, 4, NULL)		'1
	gl_sync_salesGetAttribute.Parameters.Append gl_sync_salesGetAttribute.CreateParameter("ValueType", adInteger, adParamOutput, 4, NULL)		'2
	gl_sync_salesGetAttribute.Parameters.Append gl_sync_salesGetAttribute.CreateParameter("Description", advarChar, adParamOutput, 75, NULL)	'3
	gl_sync_salesGetAttribute.Parameters.Append gl_sync_salesGetAttribute.CreateParameter("Required", adBoolean, adParamOutput, 1, NULL)		'4
	gl_sync_salesGetAttribute.Parameters.Append gl_sync_salesGetAttribute.CreateParameter("Comparable", adBoolean, adParamOutput, 1, 0)			'5
	gl_sync_salesGetAttribute.Parameters.Append gl_sync_salesGetAttribute.CreateParameter("DefaultSuffix", adVarChar, adParamOutput, 15, 0)		'6
	gl_sync_salesGetAttribute.ActiveConnection = GetSQLConnection(ctLegacy)

	if live then
		set gl_sync_cmdUpdateAttribute = CreateObject("ADODB.Command")
		set gl_sync_cmdUpdateAttribute = CreateStoredProcCommand("WebDB.dbo.Attribute_UpdateAttributeSync")
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("ProductID", adInteger, adParamInput, 4, NULL)	'1
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("AttributeID", adInteger, adParamInput, 4, NULL)	'2
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("Value", adVarChar, adParamInput, 4000, NULL)		'3
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("NumericValue", adDouble, adParamInput, 53, NULL)	'4
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("UoMID", adInteger, adParamInput, 4, NULL)		'5
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("Status", adInteger, adParamInput, 4, NULL)		'6
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("AddOnly", adBoolean, adParamInput, 1, 0)		'7
		gl_sync_cmdUpdateAttribute.ActiveConnection = GetSQLConnection(ctReadWrite)
	else
		set gl_sync_cmdUpdateAttribute = CreateObject("ADODB.Command")
		set gl_sync_cmdUpdateAttribute = CreateStoredProcCommand("WebDB.dbo.Attribute_UpdateAttributeStagingSync")
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("ProductID", adInteger, adParamInput, 4, NULL)	'1
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("AttributeID", adInteger, adParamInput, 4, NULL)	'2
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("Value", adVarChar, adParamInput, 4000, NULL)		'3
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("NumericValue", adDouble, adParamInput, 53, NULL)	'4
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("UoMID", adInteger, adParamInput, 4, NULL)		'5
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("Status", adInteger, adParamInput, 4, NULL)		'6
		gl_sync_cmdUpdateAttribute.Parameters.Append gl_sync_cmdUpdateAttribute.CreateParameter("AddOnly", adBoolean, adParamInput, 1, 0)		'7
		gl_sync_cmdUpdateAttribute.ActiveConnection = GetSQLConnection(ctReadWrite)
	end if


	Set gl_sync_RS = CreateObject("ADODB.RecordSet")
	gl_sync_Rs.ActiveConnection = GetSQLConnection(ctReadWrite)
	Set gl_sync_salesRS = CreateObject("ADODB.RecordSet")
	09/13/2001.ActiveConnection = GetSQLConnection(ctLegacy)
							   	

	gl_sync_RS.Open "Select AttributeID from WebDB.dbo.Attribute_Attributes where AttributeKey='46CAAC2F37FD11D4B7050008C7C55D36'"
	 gl_sync_NameID = gl_sync_RS(0)
	gl_sync_RS.Close

	gl_sync_RS.Open "select AttributeID from WebDB.dbo.Attribute_Attributes where AttributeKey='46CA8CDB37FD11D4B7050008C7C55D36'"
	gl_sync_ObjectiveID = gl_sync_RS(0)
	gl_sync_RS.Close

	gl_sync_RS.Open "select AttributeID from WebDB.dbo.Attribute_Attributes where AttributeKey='46CA8CDC37FD11D4B7050008C7C55D36'"
	gl_sync_SubjectiveID = gl_sync_RS(0)
	gl_sync_RS.Close

	gl_sync_RS.Open "select AttributeID from WebDB.dbo.Attribute_Attributes where AttributeKey='46CA8CDE37FD11D4B7050008C7C55D36'"
	gl_sync_BodyID = gl_sync_RS(0)
	gl_sync_RS.Close

	gl_sync_RS.Open "select AttributeID from WebDB.dbo.Attribute_Attributes where AttributeKey='82AAE31D3E1311D4B7060008C7C55D36'"
	gl_sync_PhotoFileID = gl_sync_RS(0)
	gl_sync_RS.Close

	gl_sync_RS.Open "select AttributeID from WebDB.dbo.Attribute_Attributes where AttributeKey='82AAE31C3E1311D4B7060008C7C55D36'"
	gl_sync_PhotoCaptionID = gl_sync_RS(0)
	gl_sync_RS.Close




	aryEDCs = Split(EDCList, ",")
	if isArray(aryEDCs) then

		for index = lbound(aryEDCs) to ubound(aryEDCs)
			updateProductbyEDC aryEDCs(index), live
		next
	end if

	SyncAttributesEx=NULL
end function

	Sub UpdateProductbyEDC(Item_EDC, live)
		dim ProductID, attributeID, UnitofMeasureID, status, sql
		dim cmd, AttributeIDs, AttributeCodes
		dim RegEx
		
		set RegEx = new RegExp
		Regex.Pattern = "[A-Za-z0-9]"
		
		
		gl_sync_cmdGetProductIDfromCode.Parameters(1) = Item_EDC
		gl_sync_cmdGetProductIDfromCode.Execute
		
		ProductID =  gl_sync_cmdGetProductIDfromCode.Parameters(2)
		
		if isNull(ProductID) then 
			exit sub
		end if

		' DRB Both of these statements return a list of attributeIDs, just from a different location
		if live then
			sql = "Select Attribute as ID from AdvertisementsDB..Item_Edc_Attributes where EDC_CODE = " & (Item_EDC) & " and Value <> '(value list)'"
		else
			sql = "Select ID from AdvertisementsDB..ItemEdcAttributes where ItemEDC = " & (Item_EDC) & " and Value <> '(value list)'"
		end if
		gl_sync_salesRS.Open sql

		'build a comma delimited list of C/S attributeIDs
		if not gl_sync_salesRS.eof then
			AttributeCodes = gl_sync_salesRS("ID")
			gl_sync_salesRS.movenext
		end if
		while not gl_sync_salesRS.eof 
			AttributeCodes = AttributeCodes & ", " & gl_sync_salesRS("ID")
			gl_sync_salesRS.movenext
		wend
		gl_sync_salesRS.Close

		AttributeIDs = gl_sync_NameID & ", " & gl_sync_BodyID & ", " & gl_sync_ObjectiveID & ", " & gl_sync_SubjectiveID & ", " & gl_sync_PhotoFileID & ", " & gl_sync_PhotoCaptionID 

		if not len(AttributeCodes) > 0 then
			AttributeCodes = "-1"
		end if
		
		'Remove all Attributes in staging or live, except those that are defined in the old system(s)...
			gl_sync_cmdRemoveAllAttributesExcept.Parameters(1) = ProductID
			gl_sync_cmdRemoveAllAttributesExcept.Parameters(2) = AttributeIDs
			gl_sync_cmdRemoveAllAttributesExcept.Parameters(3) = AttributeCodes
			gl_sync_cmdRemoveAllAttributesExcept.Execute
			
		'get Marketing Copy 
		if live then
			sql = "Select Name as ItemName, Objective, subjective, photo_file as photofile, photo_caption as photocaption, BLOB as marketingcopy from AdvertisementsDB..Item_EDC_MARKETING_COPY where EDC_CODE = " & (Item_EDC)
		else
			sql = "Select itemName, Objective, subjective, photofile, photocaption, complete, approved, marketingcopy, rejected from AdvertisementsDB..IEMC where ItemEDC = " & (Item_EDC)
		end if
		
		gl_sync_salesRS.Open sql

		'Reconcile the marketing copy
		if not gl_sync_salesRS.eof then
			if not live then
				if gl_sync_salesRS("Approved") = "Y" then
					Status = 3
				elseif gl_sync_salesRS("Rejected") = "Y" then
					Status = 2
				elseif gl_sync_salesRS("Complete") = "Y" then
					Status = 1
				else
					Status = 0
				end if
			else
				Status = null
			end if
			
			' gl_sync_cmdUpdateAttribute parameters for both live and stage
			' 0 RETURN_VALUE
			' 1 ProductID 
			' 2 AttributeID
			' 3 Value 
			' 4 NumericValue 
			' 5 UOMID 
			' 6 Status 
			' 7 AddOnly 

			gl_sync_cmdUpdateAttribute.Parameters(1) = ProductID
			gl_sync_cmdUpdateAttribute.Parameters(5) = NULL
			gl_sync_cmdUpdateAttribute.Parameters(6) = Status
			gl_sync_cmdUpdateAttribute.Parameters(7) = 0
			
			if(not isnull(gl_sync_salesRS("ItemName"))) then
				if(RegEx.Test(gl_sync_salesRS("ItemName"))) then
					gl_sync_cmdUpdateAttribute.Parameters(2) =  gl_sync_NameID
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("ItemName")
					gl_sync_cmdUpdateAttribute.Execute 
				end if
			else
				gl_sync_cmdRemoveAttribute.Parameters(1) = ProductID
				gl_sync_cmdRemoveAttribute.Parameters(2) = gl_sync_NameID
				gl_sync_cmdRemoveAttribute.Execute
			end if
		
			if(not isnull(gl_sync_salesRS("Objective"))) then
				if(RegEx.Test(gl_sync_salesRS("Objective"))) then
					gl_sync_cmdUpdateAttribute.Parameters(2) = gl_sync_ObjectiveID
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("Objective")
					gl_sync_cmdUpdateAttribute.Execute 
				end if
			else
				gl_sync_cmdRemoveAttribute.Parameters(1) = ProductID
				gl_sync_cmdRemoveAttribute.Parameters(2) = gl_sync_ObjectiveID
				gl_sync_cmdRemoveAttribute.Execute
			end if
		
			if(not isnull(gl_sync_salesRS("Subjective"))) then
				if(RegEx.Test(gl_sync_salesRS("Subjective"))) then
					gl_sync_cmdUpdateAttribute.Parameters(2) = gl_sync_SubjectiveID
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("Subjective")
					gl_sync_cmdUpdateAttribute.Execute 
				end if
			else
				gl_sync_cmdRemoveAttribute.Parameters(1) = ProductID
				gl_sync_cmdRemoveAttribute.Parameters(2) = gl_sync_SubjectiveID
				gl_sync_cmdRemoveAttribute.Execute
			end if
			
			if(not isnull(gl_sync_salesRS("MarketingCopy"))) then
				if(RegEx.Test(gl_sync_salesRS("MarketingCopy"))) then
					gl_sync_cmdUpdateAttribute.Parameters(2) = gl_sync_BodyID
					gl_sync_cmdUpdateAttribute.Parameters(3) = left(Trim(gl_sync_salesRS("MarketingCopy")),4000)
					gl_sync_cmdUpdateAttribute.Execute 
				end if
			else
				gl_sync_cmdRemoveAttribute.Parameters(1) = ProductID
				gl_sync_cmdRemoveAttribute.Parameters(2) = gl_sync_BodyID
				gl_sync_cmdRemoveAttribute.Execute
			end if
					
			if(not isnull(gl_sync_salesRS("PhotoFile"))) then
				if(RegEx.Test(gl_sync_salesRS("photoFile"))) then
					gl_sync_cmdUpdateAttribute.Parameters(2) = gl_sync_PhotoFileID
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("photoFile")
					gl_sync_cmdUpdateAttribute.Execute 
				end if
			else
				gl_sync_cmdRemoveAttribute.Parameters(1) = ProductID
				gl_sync_cmdRemoveAttribute.Parameters(2) = gl_sync_PhotoFileID
				gl_sync_cmdRemoveAttribute.Execute
			end if
		
			if(not isnull(gl_sync_salesRS("PhotoCaption"))) then
				if(RegEx.Test(gl_sync_salesRS("PhotoCaption"))) then
					gl_sync_cmdUpdateAttribute.Parameters(2) = gl_sync_PhotoCaptionID
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("PhotoCaption")
					gl_sync_cmdUpdateAttribute.Execute 
				end if
			else
				gl_sync_cmdRemoveAttribute.Parameters(1) = ProductID
				gl_sync_cmdRemoveAttribute.Parameters(2) = gl_sync_PhotoCaptionID
				gl_sync_cmdRemoveAttribute.Execute
			end if
		end if

		gl_sync_salesRS.Close

		'select attributes that are not "(value list)" from salessql for EDC
		'get unit of measure id from suffix
		if live then
			sql = "select Attribute as ID, Value, Suffix from advertisementsdb..Item_EDC_Attributes where Value <> '(value list)' and EDC_CODE = " & Item_EDC
		else
			sql = "select ID, Value, Suffix from advertisementsdb..ItemEDCAttributes where Value <> '(value list)' and ItemEDC = " & Item_EDC
		end if
		
		gl_sync_salesRS.Open sql

		while not gl_sync_salesRS.eof
		
			'get attributeID from old attributeid (attributecode)
			
			gl_sync_cmdUpdateAttribute.Parameters(1) = ProductID
			
			gl_sync_cmdGetAttributeIDfromCode.Parameters(1) = gl_sync_salesRS("ID")
			gl_sync_cmdGetAttributeIDfromCode.Execute
			AttributeID = gl_sync_cmdGetAttributeIDfromCode.Parameters(2)
			
			'if attribute is not found in the web system, create it
			if isnull(AttributeID) and gl_sync_salesRS("ID") <> 0 then

				gl_sync_salesGetAttribute.Parameters(1) = gl_sync_salesRS("ID")
				gl_sync_salesGetAttribute.Execute
				if not isNull(gl_sync_salesGetAttribute.Parameters(3)) then
					gl_sync_cmdAddAttribute.Parameters(1) = gl_sync_salesGetAttribute.Parameters(3) 'Name 
					gl_sync_cmdAddAttribute.Parameters(2) = NULL									'Description
					
					if gl_sync_salesGetAttribute.Parameters(2) = 0 then
						gl_sync_cmdAddAttribute.Parameters(3) = 3	'Type Yes/Nos are mapped to Select Texts
					elseif gl_sync_salesGetAttribute.Parameters(2) = 4 then
						gl_sync_cmdAddAttribute.Parameters(3) = 5	'Type Multi-Texts
					elseif gl_sync_salesGetAttribute.Parameters(2) = 5 then
						gl_sync_cmdAddAttribute.Parameters(3) = 2	'Type Ratios are really free numbers
					else 
						gl_sync_cmdAddAttribute.Parameters(3) = gl_sync_salesGetAttribute.Parameters(2)	'Type (every thing else maps through directly)
					end if
					
					if not isNull(gl_sync_salesGetAttribute.Parameters(6)) then
						gl_sync_cmdGetUnitofMeasureIDfromName.Parameters(1) = gl_sync_salesGetAttribute.Parameters(6)
						gl_sync_cmdGetUnitofMeasureIDfromName.Execute 
						gl_sync_cmdAddAttribute.Parameters(4) = gl_sync_cmdGetUnitofMeasureIDfromName.Parameters(2)	'DefaultUnitOfMeasureID
					else
						gl_sync_cmdAddAttribute.Parameters(4) = NULL	'DefaultUnitOfMeasureID
					end if
					
					gl_sync_cmdAddAttribute.Parameters(5) = gl_sync_salesGetAttribute.Parameters(4)	'Required
					gl_sync_cmdAddAttribute.Parameters(6) = gl_sync_salesGetAttribute.Parameters(5)	'Comparable
					gl_sync_cmdAddAttribute.Parameters(7) = gl_sync_salesGetAttribute.Parameters(1)	'AttributeCode
					gl_sync_cmdAddAttribute.Execute
					
					AttributeID = gl_sync_cmdAddAttribute.Parameters(8)
					
				end if
			end if			

			gl_sync_cmdGetUnitofMeasureIDfromName.Parameters(1) = gl_sync_salesRS("Suffix")
			gl_sync_cmdGetUnitofMeasureIDfromName.Execute 
			UnitOfMeasureID = gl_sync_cmdGetUnitofMeasureIDfromName.Parameters(2)
			
			
			
			gl_sync_cmdUpdateAttribute.Parameters(2) = AttributeID
			if len(gl_sync_salesRS("Value")) > 0 then
				'if Unit of measure is not found, fold it into the value.
				if len(gl_sync_salesRS("Suffix")) > 0 and isNull(UnitOfMeasureID) then
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("Value") + " " + gl_sync_salesRS("Suffix")
				else
					gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("Value")
				end if
			else
				gl_sync_cmdUpdateAttribute.Parameters(3) = NULL
			end if	

			if isNumeric(gl_sync_salesRS("Value")) then
				gl_sync_cmdUpdateAttribute.Parameters(4) = gl_sync_salesRS("Value")
			else
				gl_sync_cmdUpdateAttribute.Parameters(4) = NULL
			end if
			gl_sync_cmdUpdateAttribute.Parameters(5) = UnitOfMeasureID

			if not isnull(AttributeID) then
				gl_sync_cmdUpdateAttribute.Execute 
			end if		
		
			gl_sync_salesRS.movenext
		wend	

		gl_sync_salesRS.Close

		'select attributes that are "(value list)" from salessql for EDC
		if live then
			sql = "select ID, Value from advertisementsdb..Item_Value_List where EDC_Code = " & Item_EDC
		else
			sql = "select ID, Value from advertisementsdb..ItemValueList where ItemEDC = " & Item_EDC
		end if
		gl_sync_cmdUpdateAttribute.Parameters(7) = 1
		
		gl_sync_salesRS.Open sql

		while not gl_sync_salesRS.eof
			gl_sync_cmdGetAttributeIDfromCode.Parameters(1) = gl_sync_salesRS("ID")
			gl_sync_cmdGetAttributeIDfromCode.Execute
			AttributeID = gl_sync_cmdGetAttributeIDfromCode.Parameters(2)

			'if attribute is not found, create it
			if isnull(AttributeID) and gl_sync_salesRS("ID") <> 0 then
				gl_sync_salesGetAttribute.Parameters(1) = gl_sync_salesRS("ID")
				gl_sync_salesGetAttribute.Execute
				if not isNull(gl_sync_salesGetAttribute.Parameters(3)) then
					gl_sync_cmdAddAttribute.Parameters(1) = gl_sync_salesGetAttribute.Parameters(3) 'Name 
					gl_sync_cmdAddAttribute.Parameters(2) = NULL									'Description
					gl_sync_cmdAddAttribute.Parameters(3) = 5										'Type Multi-Texts
					gl_sync_cmdAddAttribute.Parameters(4) = NULL									'DefaultUnitOfMeasureID
					gl_sync_cmdAddAttribute.Parameters(5) = gl_sync_salesGetAttribute.Parameters(4)	'Required
					gl_sync_cmdAddAttribute.Parameters(6) = gl_sync_salesGetAttribute.Parameters(5)	'Comparable
					gl_sync_cmdAddAttribute.Parameters(7) = gl_sync_salesGetAttribute.Parameters(1)	'AttributeCode
					gl_sync_cmdAddAttribute.Execute
					
					AttributeID = gl_sync_cmdAddAttribute.Parameters(8)
					
				end if
			end if			
				
			if not isnull(AttributeID) then
				gl_sync_cmdUpdateAttribute.Parameters(1) = ProductID
				gl_sync_cmdUpdateAttribute.Parameters(2) = AttributeID
				if(not isnull(gl_sync_salesRS("Value"))) then
					if RegEx.Test(gl_sync_salesRS("Value")) then
						gl_sync_cmdUpdateAttribute.Parameters(3) = gl_sync_salesRS("Value")
						if isNumeric(gl_sync_salesRS("Value")) then
							gl_sync_cmdUpdateAttribute.Parameters(4) = gl_sync_salesRS("Value")
						else
							gl_sync_cmdUpdateAttribute.Parameters(4) = NULL
						end if
						gl_sync_cmdUpdateAttribute.Parameters(5) = NULL
						gl_sync_cmdUpdateAttribute.Execute 
					end if
				end if
			end if

			gl_sync_salesRS.movenext
		wend

		gl_sync_salesRS.Close
	end Sub
%>
