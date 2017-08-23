<%
	'	DESCRIPTION:
	'		Functions for product information.
	'	ASSUMPTIONS:
	'		/global/subs/sqlconn.inc
	'		/global/subs/global.inc

	' Product Database Cluster Property
	Dim ConnectionType_Product
	ConnectionType_Product = ctReadWrite

	' Logical Catalog Constants
	Const prMasterLogicalCatalog	= "2EB25A674CA011D4B7070008C7C55D36"		' Master Logical Catalog for all cdw/cdwg products
	Const prCDWLogicalCatalog		= "25FE01674E7D11D4B7070008C7C55D36"		' Logical Catalog for all cdw products
	Const prCDWGLogicalCatalog		= "E81F6A9784EE11D4B70B0008C7C55D36"		' Logical Catalog for all cdwg products
	Const prCDWCALogicalCatalog		= "0AE5C829F30B488AAF1D9AB6639BE5D1"		' Logical Catalog for all cdw.ca products

	' Catalog Schema Constants
	Const prMasterCatalogSchema		= "2EB255BE4CA011D4B7070008C7C55D36"		' Master Catalog Schema for all cdw/cdwg products
	Const prCDWCatalogSchema		= "25FE016B4E7D11D4B7070008C7C55D36"		' Catalog Schema for all cdw products
	Const prCDWGCatalogSchema		= "E81F6AA384EE11D4B70B0008C7C55D36"		' Catalog Schema for all cdwg products
	Const prCDWCACatalogSchema		= "0AE5C829F30B488AAF1D9AB6639BE5D1"		' Catalog Schema for all cdw.ca products

	' Physical Catalog Constants
	Const prMasterCatalog			= "1C2D3ABA2C3F11D4B90100805FE66F01"		' Physical Catalog for all cdw/cdwg products
	Const prCDWCatalog				= "????????????????????????????????"		' Catalog for all cdw products {currently not needed}
	Const prCDWGCatalog				= "????????????????????????????????"		' Catalog for all cdwg products {currently not needed}

	' Physical Bad Catalog Constants
	Const prCDWBadCatalog			= "E81F6AAB84EE11D4B70B0008C7C55D36"		' Catalog for all bad cdw products
	Const prCDWGBadCatalog			= "E81F6AAA84EE11D4B70B0008C7C55D36"		' Catalog for all bad cdwg products
	Const prCDWCABadCatalog			= "624D74378F954349A58EA28AF8008703"		' Catalog for all bad cdw.ca products

	'Stock Status Constants 
	' - if these constants are changed /global/gui/messages/shipstat.asp will also need to be updated
	Const prStockStatusNameIn					= "Same Day"
	Const prStockStatusNameOneToThreeDays		= "1-3 Days"
	Const prStockStatusNameThreeToSixDays		= "3-6 Days"
	Const prStockStatusNameFourToSixDays		= "4-6 Days"
	Const prStockStatusNameOneToTwoWeeks		= "1-2 Weeks"
	Const prStockStatusNameTwoOrMoreWeeks		= "2+ Weeks"
	Const prStockStatusNameSpecialOrder			= "Call"
	Const prStockStatusNamePreOrder				= "Pre-Order"

	Const prStockStatusDescriptionIn				= "Product is in-stock. Order expected to ship same day if order is released by 4pm CT."
	Const prStockStatusDescriptionInCA				= "Product is in-stock. Order expected to ship same day if order is released by 5pm ET."
	Const prStockStatusDescriptionOneToThreeDays	= "Product is expected to be available for shipment within 1-3 days. "
	Const prStockStatusDescriptionThreeToSixDays	= "Product is expected to be available for shipment within 3-6 days. "
	Const prStockStatusDescriptionFourToSixDays		= "Product is expected to be available for shipment within 4-6 days. "
	Const prStockStatusDescriptionOneToTwoWeeks		= "Product is expected to be available for shipment within 1-2 weeks. "
	Const prStockStatusDescriptionTwoOrMoreWeeks	= "Product is expected to be available for shipment within 2 weeks or more. "
	Const prStockStatusDescriptionSpecialOrder		= "Please contact your account manager for estimated delivery time."
	Const prStockStatusDescriptionPreOrder			= "This product is not yet available, but we are accepting pre-orders.  Once product is available, orders are fulfilled in the order in which they were received. Please contact your account manager for more details."

	'Alert Constants
	Const alInStock			= "72A3CECA413611D4B7060008C7C55D36"
	Const alPriceChange		= "72A3CECC413611D4B7060008C7C55D36"

'***************************************************************
'	DESCRIPTION:
'		Adds a product.
'		Returns ProductID/Null

Function AddProduct(Name, Description, CompanyPartNumber, DateSellStart, DateSellStop)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(IsEmpty(CompanyPartNumber)) and Not(IsNull(CompanyPartNumber)) and Len(CompanyPartNumber) > 0) Then
			If(Not(IsEmpty(DateSellStart)) and (IsNull(DateSellStart) or IsDate(DateSellStart))) Then
				If(Not(IsEmpty(DateSellStop)) and (IsNull(DateSellStop) or IsDate(DateSellStop))) Then
					If(Not(Len(Description) > 0)) Then Description = Null End If
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddProduct")
					Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)							'1
					Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)			'2
					Cmd.Parameters.Append Cmd.CreateParameter("@CompanyPartNumber", adVarChar, adParamInput, 50, CompanyPartNumber)	'3
					Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamInput,, DateSellStart)		'4
					Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStop", adDBTimeStamp, adParamInput,, DateSellStop)			'5
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)								'6
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
	AddProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product.
'		Error True/False

'Function DeleteProduct(ProductID)
'	Dim returnValue
'	returnValue = True
'	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
'		Dim Cmd
'		Call OpenSQLConnection(ctReadWrite)
'		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProduct")
'		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
'		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
'		If(Cmd.Parameters(0).Value = 0) Then
'			returnValue = False
'		End If
'		Set Cmd = Nothing
'		Call CloseSQLConnection(ctReadWrite)
'	End If
'	DeleteProduct = returnValue
'End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a product.
'		Error True/False

Function UpdateProduct(ProductID, Name, Description, CompanyPartNumber, DateSellStart, DateSellStop)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(IsEmpty(CompanyPartNumber)) and Not(IsNull(CompanyPartNumber)) and Len(CompanyPartNumber) > 0) Then
				If(Not(IsEmpty(DateSellStart)) and (IsNull(DateSellStart) or IsDate(DateSellStart))) Then
					If(Not(IsEmpty(DateSellStop)) and (IsNull(DateSellStop) or IsDate(DateSellStop))) Then
						If(Not(Len(Description) > 0)) Then Description = Null End If
						Dim Cmd
						Call OpenSQLConnection(ctReadWrite)
						Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_UpdateProduct")
						Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
						Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)							'2
						Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)			'3
						Cmd.Parameters.Append Cmd.CreateParameter("@CompanyPartNumber", adVarChar, adParamInput, 50, CompanyPartNumber)	'4
						Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamInput,, DateSellStart)		'5
						Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStop", adDBTimeStamp, adParamInput,, DateSellStop)			'6
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
	UpdateProduct = returnValue
End Function


'****************************************************************************************************************************************
' Description: Returns product information.
' Created By: David Tzau
' Date: Oct.25.2001
'****************************************************************************************************************************************
Const prProductID						= 0
Const prProductKey						= 1
Const prProductCode						= 2
Const prProductName						= 3
Const prProductFriendlyName				= 4
Const prProductDescription				= 5
Const prProductFriendlyDescription		= 6
Const prProductCompanyPartNumber		= 7
Const prProductManufactureID			= 8
Const prProductManufactureKey			= 9
Const prProductManufactureCode			= 10
Const prProductManufactureName			= 11
Const prProductManufacturePartNumber	= 12
Const prProductWeight					= 13
Const prProductHeight					= 14
Const prProductWidth					= 15
Const prProductLength					= 16
Const prProductNonReturnable			= 17
Const prProductDropShipOnly				= 18
Const prProductDateSellStart			= 19
Const prProductDateSellStop				= 20
Const prProductDateCreated				= 21
Const prProductDateModified				= 22
Const prProductImageEDC					= 23
Const prProductSpinSetName				= 24
Const prProductCustomerSpecificEDC		= 25

Function GetProduct(ProductID)	
	Dim returnValue, ary(25), Par
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProduct")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductKey", adChar, adParamOutput, 32)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamOutput, 15)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyName", adVarChar, adParamOutput, 150)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'7
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyDescription", adVarChar, adParamOutput, 500)	'8
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyPartNumber", adVarChar, adParamOutput, 50)		'9
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)				'10
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureKey", adChar, adParamOutput, 32)				'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamOutput, 15)			'12
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureName", adVarChar, adParamOutput, 75)			'13
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturePartNumber", adVarChar, adParamOutput, 50)	'14
		Set Par = Cmd.CreateParameter("@Weight", adNumeric, adParamOutput)									'15
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Height", adNumeric, adParamOutput)									'16
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Width", adNumeric, adParamOutput)									'17
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Length", adNumeric, adParamOutput)									'18
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Cmd.Parameters.Append Cmd.CreateParameter("@NonReturnable", adBoolean, adParamOutput)				'19
		Cmd.Parameters.Append Cmd.CreateParameter("@DropShipOnly", adInteger, adParamOutput)				'20
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamOutput)			'21
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStop", adDBTimeStamp, adParamOutput)			'22
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'23
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'24
		Cmd.Parameters.Append Cmd.CreateParameter("@ImageEDC", adVarChar, adParamOutput, 50)				'25
		Cmd.Parameters.Append Cmd.CreateParameter("@SpinSetName", adVarChar, adParamOutput, 50)				'26
		Cmd.Parameters.Append Cmd.CreateParameter("@CustomerSpecificEDC", adBoolean, adParamOutput)			'27

		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)

		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductID)					= ProductID
			ary(prProductKey)					= Cmd.Parameters(2).Value
			ary(prProductCode)					= Cmd.Parameters(3).Value
			ary(prProductName)					= Cmd.Parameters(4).Value
			ary(prProductFriendlyName)			= Cmd.Parameters(5).Value
			ary(prProductDescription)			= Cmd.Parameters(6).Value
			ary(prProductFriendlyDescription)	= Cmd.Parameters(7).Value
			ary(prProductCompanyPartNumber)		= Cmd.Parameters(8).Value
			ary(prProductManufactureID)			= Cmd.Parameters(9).Value
			ary(prProductManufactureKey)		= Cmd.Parameters(10).Value
			ary(prProductManufactureCode)		= Cmd.Parameters(11).Value
			ary(prProductManufactureName)		= Cmd.Parameters(12).Value
			ary(prProductManufacturePartNumber)	= Cmd.Parameters(13).Value
			ary(prProductWeight)				= Cmd.Parameters(14).Value
			ary(prProductHeight)				= Cmd.Parameters(15).Value
			ary(prProductWidth)					= Cmd.Parameters(16).Value
			ary(prProductLength)				= Cmd.Parameters(17).Value
			ary(prProductNonReturnable)			= Cmd.Parameters(18).Value
			ary(prProductDropShipOnly)			= Cmd.Parameters(19).Value
			ary(prProductDateSellStart)			= Cmd.Parameters(20).Value
			ary(prProductDateSellStop)			= Cmd.Parameters(21).Value
			ary(prProductDateCreated)			= Cmd.Parameters(22).Value
			ary(prProductDateModified)			= Cmd.Parameters(23).Value
			ary(prProductImageEDC)				= Cmd.Parameters(24).Value
			ary(prProductSpinSetName)			= Cmd.Parameters(25).Value
			ary(prProductCustomerSpecificEDC)	= Cmd.Parameters(26).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProduct = returnValue		
End Function

'****************************************************************************************************************************************
' Description: Returns products friendly name and description
' Created By: Paul Haan
' Date: July.24.2002
'****************************************************************************************************************************************
Const prProductFriendlyProductID						= 0
Const prProductFriendlyProductFriendlyName				= 1
Const prProductFriendlyProductFriendlyDescription		= 2

Function GetProductFriendlyInformation(ProductID)
	Dim returnValue, ary(2)
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductFriendlyInformation")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyName", adVarChar, adParamOutput, 150)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyDescription", adVarChar, adParamOutput, 500)	'3
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)

		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductFriendlyProductID)						= ProductID
			ary(prProductFriendlyProductFriendlyName)			= Cmd.Parameters(2).Value
			ary(prProductFriendlyProductFriendlyDescription)	= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductFriendlyInformation = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all products (i.e. table dump).
'		Note: Constants defined above
'		RecordSet (to big for an array)

Function GetProducts()
	Dim Cmd, resultSet
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProducts")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
	Call CloseSQLConnection(ConnectionType_Product)
	Set GetProducts = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns product information for multiple products.
'		Pass a 1x array, 2x array, or recordset with a column
'		index for the later two specifing ProductID's.  Pass a
'		string when you wish to build the where clause yourself.
'		Returns Array/Null

'	Note: Constants defined above

' Order by constants
Const prProductOrderByProductID			= "p.ProductID"
Const prProductOrderByProductCode		= "p.ProductCode"
Const prProductOrderByFriendlyName		= "p.FriendlyName"
Const prProductOrderByPriceAdvertised	= "pp.PriceAdvertised"

Function GetProductBulk(ProductIDs, ProductIDIndex, ProductOrderBy)

	Dim returnValue, resultSet, TestDimension, ProductIDList, i
	returnValue = Null
	If(Not(IsEmpty(ProductIDs)) and Not(IsNull(ProductIDs))) Then
		If(Not(IsEmpty(ProductIDIndex)) and (IsNull(ProductIDIndex) or IsNumeric(ProductIDIndex))) Then
			If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If
			If(IsArray(ProductIDs)) Then
				If(Not(Is2xArray(ProductIDs))) Then
					' 1x Array
					ProductIDList = Join(ProductIDs, ",")
					ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
				Else
					' 2x Array
					ProductIDList = ""
					For i = Lbound(ProductIDs, 2) To Ubound(ProductIDs, 2)
						If(i = Ubound(ProductIDs, 2)) Then
							ProductIDList = ProductIDList & ProductIDs(ProductIDIndex, i)
						Else
							ProductIDList = ProductIDList & ProductIDs(ProductIDIndex, i) & ","
						End If
					Next
					ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
				End If
			ElseIf(IsObject(ProductIDs)) Then
				' RecordSet
				If(ProductIDs.State = adstateOpen) Then
					ProductIDs.MoveFirst
					ProductIDList = ""
					While(Not(ProductIDs.EOF))
						ProductIDList = ProductIDList & ProductIDs(ProductIDIndex) & ","
						ProductIDs.MoveNext
					Wend
					ProductIDList = Left(ProductIDList, Len(ProductIDList) - 1)
					ProductIDs.MoveFirst
					ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
				End If
			ElseIf(VarType(ProductIDs) = vbString) Then
				' String
				ProductIDList = ProductIDs
			End If
			If(Not(IsEmpty(ProductIDList)) and Not(IsNull(ProductIDList)) and (Len(ProductIDList) > 0)) Then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Product)
				Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductBulk")
				Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, ProductIDList)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)	'3
				Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
				If(resultSet.State = adstateOpen) Then
					If(Not(resultSet.EOF)) Then
						returnValue = resultSet.GetRows
					End If
					resultSet.Close
				End If
				Set resultSet = Nothing
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Product)
			End If
		End If
	End If
	GetProductBulk = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a ProductID from the guid.
'		Returns ProductID/Null

Function GetProductID(ProductKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ProductKey)) and Not(IsNull(ProductKey)) and Len(ProductKey) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductKey", adChar, adParamInput, 32, ProductKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a ProductID from the ProductCode.
'		Returns ProductID/Null

Function GetProductIDByCode(ProductCode)
	Dim returnValue, productCodeLength
	returnValue = Null
	productCodeLength = Len(ProductCode)
	If(Not(IsEmpty(ProductCode)) and Not(IsNull(ProductCode)) and productCodeLength > 0) Then
	
		'Limit the ProductCode to 15 characters to match sproc input.  Anything larger will
		'produce a SQL error.
		If productCodeLength > 15 Then
			ProductCode = Mid(ProductCode, 1, 15)
		End If
		
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductIDByCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamInput, 15, ProductCode)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)					'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductIDByCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a ProductID from the CompanyPartNumber.
'		Returns ProductID/Null

Function GetProductIDByCompanyPartNumber(CompanyPartNumber)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CompanyPartNumber)) and Not(IsNull(CompanyPartNumber)) and Len(CompanyPartNumber) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductIDByCompanyPartNumber")
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyPartNumber", adVarChar, adParamInput, 50, CompanyPartNumber)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)								'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductIDByCompanyPartNumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a ProductID from the ManufacturePartNumber.
'		The ManufactureID optionally restricts the search to
'		that manufacture a Null will search the whole product
'		catalog and does not guarantee uniqueness.
'		Returns ProductID/Null

Function GetProductIDByManufacturePartNumber(ManufacturePartNumber, ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ManufacturePartNumber)) and Not(IsNull(ManufacturePartNumber)) and Len(ManufacturePartNumber) > 0) Then
		If(Not(IsEmpty(ManufactureID)) and (IsNull(ManufactureID) or IsNumeric(ManufactureID))) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductIDByManufacturePartNumber")
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturePartNumber", adVarChar, adParamInput, 50, ManufacturePartNumber)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)										'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	GetProductIDByManufacturePartNumber = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the Product Code
'		Returns ProductID/NULL

Function GetProductCode(ProductID)
	Dim returnValue
	returnValue = null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductCode")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamOutput, 15)			'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)	
	End If
	GetProductCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a products sell start date.
'		Returns Date/Null

Function GetProductDateSellStart(ProductID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductDateSellStart")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamOutput)		'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductDateSellStart = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a ManufactureID for the product.
'		Returns ManufactureID/Null

Function GetProductManufactureID(ProductID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductManufactureID")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)			'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductManufactureID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Determines if a product can be purchased.
'		Error True/False

Function ProductPurchasable(LogicalCatalogID, ProductID)
	Dim returnValue 
	Dim TodaysDate
	Dim Product, ProductPriceAdvertised, ProductDateSellStart, ProductDateSellStop
	
	'site specific info
	Dim CompanyID
	
	'product prices array
	Dim arProductPrice

	'determine companyid
	CompanyID = GetCompanyID(GetThisProperty())
			
	returnValue = False
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Product = GetProduct(ProductID)

			If(Not(IsNull(Product))) Then
				'retrieve Product advertised price
				arProductPrice = GetProductPrices(ProductID, CompanyID)
				If IsArray(arProductPrice) Then
					ProductPriceAdvertised = arProductPrice(prProductPriceAdvertised)
				End If

				ProductDateSellStart	= Product(prProductDateSellStart)
				ProductDateSellStop		= Product(prProductDateSellStop)
				If(Not(IsEmpty(ProductPriceAdvertised)) and Not(IsNull(ProductPriceAdvertised)) and IsNumeric(ProductPriceAdvertised) and ProductPriceAdvertised <> 0) Then
					TodaysDate = Now()
					If(DateDiff("d", ProductDateSellStart, TodaysDate) >= 0) and ( (DateDiff("d", TodaysDate, ProductDateSellStop) > 0) or (IsNull(ProductDateSellStop)) ) Then
						If(LogicalCatalogHasProduct(LogicalCatalogID, ProductID)) Then
							returnValue = True
						End If
 					End If
				End If
			End If
		End If
	End If
	ProductPurchasable = returnValue
End Function




'*************************************************************** 
'	DESCRIPTION:
'		Updates a products code.
'		Error True/False

Function SetProductCode(ProductID, ProductCode)
	Dim returnValue
	returnValue = True
	If Not IsEmpty(ProductID) and Not IsNull(ProductID) and IsNumeric(ProductID) Then
		If Not IsEmpty(ProductCode) and Not IsNull(ProductCode) and Len(ProductCode) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductCode")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamInput, 15, ProductCode)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetProductCode = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products friendly information.
'		i.e. Nice human readable stuff...
'		Error True/False

Function SetProductFriendlyInformation(ProductID, FriendlyName, FriendlyDescription)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(FriendlyName)) and Not(IsNull(FriendlyName)) and Len(FriendlyName) > 0) Then
			If(Not(Len(FriendlyDescription) > 0)) Then FriendlyDescription = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductFriendlyInformation")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)							'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyName", adVarChar, adParamInput, 150, FriendlyName)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyDescription", adVarChar, adParamInput, 500, FriendlyDescription)	'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetProductFriendlyInformation = returnValue
End Function

'****************************************************************************************************************************************
' Description: Returns ImageEDC and SpinSetName
' Created By: Paul Haan
' Date: June.10.2005
'****************************************************************************************************************************************
Const prProductPhotoProductID		= 0
Const prProductPhotoImageEDC		= 1
Const prProductPhotoSpinSetName		= 2

Function GetProductPhotoInformation(ProductID)
	Dim returnValue, ary(2)
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctLegacy)
		Set Cmd = CreateSQLProcedure(ctLegacy, "ProductDB.dbo.Product_GetProductPhotoInformation")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ImageEDC", adVarChar, adParamOutput, 15)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@SpinSetName", adVarChar, adParamOutput, 50)				'3
		Call ExecuteSQLProcedure(ctLegacy, Cmd, False)

		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductPhotoProductID)	= ProductID
			ary(prProductPhotoImageEDC)		= Cmd.Parameters(2).Value
			ary(prProductPhotoSpinSetName)	= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctLegacy)
	End If
	GetProductPhotoInformation = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a products photo information.
'		This updates the data on the Internal DB.  Then it is replicated to the External DB
'		Error True/False

Function SetProductPhotoInformation(ProductID, ImageEDC, SpinSetName)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ImageEDC)) and Not(IsNull(ImageEDC)) and Len(ImageEDC) > 0) Then
			If(Not(Len(SpinSetName) > 0)) Then SpinSetName = null End If
			Dim Cmd
			Call OpenSQLConnection(ctLegacy)
			Set Cmd = CreateSQLProcedure(ctLegacy, "ProductDB.dbo.Product_SetProductPhotoInformation")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ImageEDC", adVarChar, adParamInput, 15, ImageEDC)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@SpinSetName", adVarChar, adParamInput, 50, SpinSetName)	'3
			Call ExecuteSQLProcedure(ctLegacy, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctLegacy)
		End If
	End If
	SetProductPhotoInformation = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Add the friendly name and description to the temp table (Attribute_ProductChangeQueue).
'		Returns ProductBundleID/Null

Function UpdateTemporaryProductFriendlyInformation(ProductID, FriendlyName, FriendlyDescription)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(FriendlyName)) and Not(IsNull(FriendlyName)) and Len(FriendlyName) > 0) Then
			If(Not(Len(FriendlyDescription) > 0)) Then FriendlyDescription = Null End If
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddTemporaryProductFriendlyInformation")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)							'1
				Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyName", adVarChar, adParamInput, 75, FriendlyName)					'2
				Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyDescription", adVarChar, adParamInput, 500, FriendlyDescription)	'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateTemporaryProductFriendlyInformation = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a products pricing.
'		Error True/False

Function SetProductManufactureInformation(ProductID, ManufactureID, ManufacturePartNumber)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			If(Not(IsEmpty(ManufacturePartNumber)) and Not(IsNull(ManufacturePartNumber)) and Len(ManufacturePartNumber) > 0) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductManufactureInformation")
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)							'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)					'2
				Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturePartNumber", adVarChar, adParamInput, 50, ManufacturePartNumber)	'3
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = False
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	SetProductManufactureInformation = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products physical attributes.
'		i.e. weight & dimensions
'		Error True/False

Function SetProductPhysicalAttributes(ProductID, Weight, Height, Width, Length)
	Dim returnValue, Par
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPhysicalAttributes")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Set Par = Cmd.CreateParameter("@Weight", adNumeric, adParamInput,, Weight)									'2
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Height", adNumeric, adParamInput,, Height)									'3
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Width", adNumeric, adParamInput,, Width)									'4
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Length", adNumeric, adParamInput,, Length)									'5
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPhysicalAttributes = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates Non-Returnable flag.
'		Error True/False

Function SetProductNonReturnable(ProductID, NonReturnable)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(NonReturnable)) and Not(IsNull(NonReturnable)) and IsNumeric(NonReturnable)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductNonReturnable")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@NonReturnable", adBoolean, adParamInput,, NonReturnable)	'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetProductNonReturnable = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates Drop ship only flag.
'		Error True/False

Function SetProductDropShipOnly(ProductID, DropShipOnly)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(DropShipOnly)) and Not(IsNull(DropShipOnly)) and IsNumeric(DropShipOnly)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductDropShipOnly")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DropShipOnly", adInteger, adParamInput,, DropShipOnly)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetProductDropShipOnly = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a product to a folder.
'		Returns ProductFolderAssignmentID/Null

Function AddProductFolderAssignment(ProductID, FolderID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddProductFolderAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductFolderAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Deletes a product from folder.
'		Error True/False

Function DeleteProductFolderAssignment(ProductFolderAssignmentID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductFolderAssignmentID)) and Not(IsNull(ProductFolderAssignmentID)) and IsNumeric(ProductFolderAssignmentID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductFolderAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductFolderAssignmentID", adInteger, adParamInput,, ProductFolderAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductFolderAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product from all folders in the current
'		directory.
'		Error True/False

Function DeleteProductFolderAssignments(ProductID, DirectoryID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductFolderAssignments")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteProductFolderAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all products in the current folder.
'		Array/Null

Const prProductFolderAssignmentID						= 0
Const prProductFolderAssignmentProductID				= 1
Const prProductFolderAssignmentFolderID					= 2
Const prProductFolderAssignmentProductKey				= 3
Const prProductFolderAssignmentProductCode				= 4
Const prProductFolderAssignmentName						= 5
Const prProductFolderAssignmentFriendlyName				= 6
Const prProductFolderAssignmentDescription				= 7
Const prProductFolderAssignmentFriendlyDescription		= 8
Const prProductFolderAssignmentCompanyPartNumber		= 9
Const prProductFolderAssignmentManufactureID			= 10
Const prProductFolderAssignmentManufactureKey			= 11
Const prProductFolderAssignmentManufactureCode			= 12
Const prProductFolderAssignmentManufactureName			= 13
Const prProductFolderAssignmentManufacturePartNumber	= 14
Const prProductFolderAssignmentPriceActual				= 15
Const prProductFolderAssignmentPriceLanded				= 16
Const prProductFolderAssignmentPriceSimulated			= 17
Const prProductFolderAssignmentPriceAdvertised			= 18
Const prProductFolderAssignmentPriceSuggested			= 19
Const prProductFolderAssignmentPriceOverride			= 20
Const prProductFolderAssignmentWeight					= 21
Const prProductFolderAssignmentHeight					= 22
Const prProductFolderAssignmentWidth					= 23
Const prProductFolderAssignmentLength					= 24
Const prProductFolderAssignmentNonReturnable			= 25
Const prProductFolderAssignmentDropShipOnly				= 26
Const prProductFolderAssignmentDateSellStart			= 27
Const prProductFolderAssignmentDateSellStop				= 28
Const prProductFolderAssignmentDateCreated				= 29
Const prProductFolderAssignmentDateModified				= 30

Function GetProductFolderAssignments(CompanyID, FolderID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Call OpenSQLConnection(ConnectionType_Product)
			Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductFolderAssignments")
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	GetProductFolderAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the folder id the given product is under.
'		Returns FolderID/Null

Function GetProductFolderID(ProductID, DirectoryID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(DirectoryID)) and Not(IsNull(DirectoryID)) and IsNumeric(DirectoryID)) Then
			Call OpenSQLConnection(ConnectionType_Product)
			Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductFolderID")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@DirectoryID", adInteger, adParamInput,, DirectoryID)'2
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	GetProductFolderID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a product accessory.
'		Returns ProductAccessoryID/Null

Function AddProductAccessory(ProductID, ProductChildID, UserCreated)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ProductChildID)) and Not(IsNull(ProductChildID)) and IsNumeric(ProductChildID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddProductAccessory")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductChildID", adInteger, adParamInput,, ProductChildID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@UserCreated", adVarChar, adParamInput, 50, UserCreated)			'3
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryID", adInteger, adParamOutput)					'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductAccessory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product accessory.
'		Error True/False

Function DeleteProductAccessory(ProductAccessoryID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductAccessoryID)) and Not(IsNull(ProductAccessoryID)) and IsNumeric(ProductAccessoryID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductAccessory")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryID", adInteger, adParamInput,, ProductAccessoryID)	'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductAccessory = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes all of a products accessories.
'		Error True/False

Function DeleteProductAccessories(ProductID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductAccessories")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductAccessories = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all of a products accessories.
'		Array/Null

Const prProductAccessoryID						= 0
Const prProductAccessoryProductID				= 1
Const prProductAccessoryProductKey				= 2
Const prProductAccessoryProductCode				= 3
Const prProductAccessoryName					= 4
Const prProductAccessoryFriendlyName			= 5
Const prProductAccessoryDescription				= 6
Const prProductAccessoryFriendlyDescription		= 7
Const prProductAccessoryCompanyPartNumber		= 8
Const prProductAccessoryManufactureID			= 9
Const prProductAccessoryManufactureKey			= 10
Const prProductAccessoryManufactureCode			= 11
Const prProductAccessoryManufactureName			= 12
Const prProductAccessoryManufacturePartNumber	= 13
Const prProductAccessoryPriceActual				= 14
Const prProductAccessoryPriceLanded				= 15
Const prProductAccessoryPriceSimulated			= 16
Const prProductAccessoryPriceAdvertised			= 17
Const prProductAccessoryPriceSuggested			= 18
Const prProductAccessoryPriceOverride			= 19
Const prProductAccessoryWeight					= 20
Const prProductAccessoryHeight					= 21
Const prProductAccessoryWidth					= 22
Const prProductAccessoryLength					= 23
Const prProductAccessoryNonReturnable			= 24
Const prProductAccessoryDropShipOnly			= 25
Const prProductAccessoryDateSellStart			= 26
Const prProductAccessoryDateSellStop			= 27
Const prProductAccessoryDateCreated				= 28
Const prProductAccessoryDateModified			= 29
Const prProductAccessoryUserCreated				= 30

Function GetProductAccessories(ProductID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductAccessories")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductAccessories = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all of a products accessories sorted by folders.
'		Array/Null

Const prgProductAccessoryFolderName				= 0
Const prgProductAccessoryFolderID				= 1
Const prgProductAccessoryRanked					= 2
Const prgProductAccessoryRankedAccessoryFolder	= 3
Const prgProductAccessoryID						= 4
Const prgProductAccessoryProductID				= 5
Const prgProductAccessoryProductKey				= 6
Const prgProductAccessoryProductCode			= 7
Const prgProductAccessoryName					= 8
Const prgProductAccessoryFriendlyName			= 9
Const prgProductAccessoryDescription			= 10
Const prgProductAccessoryFriendlyDescription	= 11
Const prgProductAccessoryCompanyPartNumber		= 12
Const prgProductAccessoryManufactureID			= 13
Const prgProductAccessoryManufactureKey			= 14
Const prgProductAccessoryManufactureCode		= 15
Const prgProductAccessoryManufactureName		= 16
Const prgProductAccessoryManufacturePartNumber	= 17
Const prgProductAccessoryPriceActual			= 18
Const prgProductAccessoryPriceLanded			= 19
Const prgProductAccessoryPriceSimulated			= 20
Const prgProductAccessoryPriceAdvertised		= 21
Const prgProductAccessoryPriceSuggested			= 22
Const prgProductAccessoryPriceOverride			= 23
Const prgProductAccessoryWeight					= 24
Const prgProductAccessoryHeight					= 25
Const prgProductAccessoryWidth					= 26
Const prgProductAccessoryLength					= 27
Const prgProductAccessoryNonReturnable			= 28
Const prgProductAccessoryDropShipOnly			= 29
Const prgProductAccessoryDateSellStart			= 30
Const prgProductAccessoryDateSellStop			= 31
Const prgProductAccessoryDateCreated			= 32
Const prgProductAccessoryDateModified			= 33
Const prgProductAccessoryUserCreated			= 34
Const prgProductAccessoryImageEDC				= 35
Const prgProductAccessorySpinSetName			= 36
Const prgProductAccessoryCustomerSpecificEDC	= 37

Function GetProductAccessoriesByClass(ProductID, CompanyID)	
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		'ConnectionType_Product
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductAccessoriesByClass")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)					'1
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductAccessoriesByClass = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all of a products accessory folders for a product folder.
'		Array/Null

Const aeAccessoryFolderFolderID					= 0
Const aeAccessoryFolderFolderKey				= 1
Const aeAccessoryFolderFolderCode				= 2
Const aeAccessoryFolderDirectoryID				= 3
Const aeAccessoryFolderFolderName				= 4
Const aeAccessoryFolderFolderDescription		= 5
Const aeAccessoryFolderSortOrder				= 6
Const aeAccessoryFolderFolderSortAssignmentID	= 7
Const aeAccessoryFolderFolderDateCreated		= 8
Const aeAccessoryFolderFolderDateModified		= 9

Function GetAccessoryFolders(FolderID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetAccessoryFolders")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetAccessoryFolders = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product has any accessories.
'		Returns True/False

Function ProductHasAccessories(ProductID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_ProductHasAccessories")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Accessory", adBoolean, adParamOutput)							'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	ProductHasAccessories = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product accessory exists.
'		Returns True/False

Function ProductHasAccessory(ProductID, ProductChildID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ProductChildID)) and Not(IsNull(ProductChildID)) and IsNumeric(ProductChildID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_ProductHasAccessory")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductChildID", adInteger, adParamInput,, ProductChildID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Accessory", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	ProductHasAccessory = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns an product name from the ProductAccessory.
'		Returns UnitName/Null

Function GetProductAccessoryName(ProductAccessoryID)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductAccessoryID)) and Not(IsNull(ProductAccessoryID)) and IsNumeric(ProductAccessoryID)) Then
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductAccessoryName")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryID", adInteger, adParamInput,, ProductAccessoryID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 150)	'Actually FriendlyName		'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductAccessoryName = returnValue
End Function

'***************************************************************
	'move to product2subs once it is checked back in...
Function UpdateBundle(BundleID, Name, Description)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(BundleID)) and Not(IsNull(BundleID)) and IsNumeric(BundleID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Bundle_UpdateBundle")
		Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 200, Name)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 1900, Description)	'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateBundle = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a product Bundle.
'		Returns ProductBundleID/Null

Function AddProductBundle(ProductID, ProductChildID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ProductChildID)) and Not(IsNull(ProductChildID)) and IsNumeric(ProductChildID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddProductBundle")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductChildID", adInteger, adParamInput,, ProductChildID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductBundleID", adInteger, adParamOutput)						'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddProductBundle = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product Bundle.
'		Error True/False

Function DeleteProductBundle(ProductBundleID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductBundleID)) and Not(IsNull(ProductBundleID)) and IsNumeric(ProductBundleID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductBundle")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductBundleID", adInteger, adParamInput,, ProductBundleID)	'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductBundle = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes all of a products bundles.
'		Error True/False

Function DeleteProductBundles(ProductID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductBundles")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductBundles = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all of a products bundles.
'		Array/Null

Const prProductBundleID						= 0
Const prProductBundleProductID				= 1
Const prProductBundleKey					= 2
Const prProductBundleCode					= 3
Const prProductBundleName					= 4
Const prProductBundleFriendlyName			= 5
Const prProductBundleDescription			= 6
Const prProductBundleFriendlyDescription	= 7
Const prProductBundleCompanyPartNumber		= 8
Const prProductBundleManufactureID			= 9
Const prProductBundleManufactureKey			= 10
Const prProductBundleManufactureCode		= 11
Const prProductBundleManufactureName		= 12
Const prProductBundleManufacturePartNumber	= 13
Const prProductBundleWeight					= 14
Const prProductBundleHeight					= 15
Const prProductBundleWidth					= 16
Const prProductBundleLength					= 17
Const prProductBundleNonReturnable			= 18
Const prProductBundleDropShipOnly			= 19
Const prProductBundleDateSellStart			= 20
Const prProductBundleDateSellStop			= 21
Const prProductBundleDateCreated			= 22
Const prProductBundleDateModified			= 23
Const prProductBundleImageEDC				= 24
Const prProductBundleSpinSetName			= 25
Const prProductBundleCustomerSpecificEDC	= 26

Function GetProductBundles(ProductID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductBundles")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductBundles = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product bundle exists.
'		Returns True/False

Function ProductHasBundle(ProductID, ProductChildID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(ProductChildID)) and Not(IsNull(ProductChildID)) and IsNumeric(ProductChildID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_ProductHasBundle")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductChildID", adInteger, adParamInput,, ProductChildID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Bundle", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	ProductHasBundle = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a products pricing.
'		Returns ProductPriceID/Null

Function AddProductPricing(ProductID, CompanyID, PriceActual, PriceLanded, PriceSimulated, PriceAdvertised, PriceSuggested, PriceOverride)
	Dim returnValue, Cmd
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) and _
	  (Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddProductPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceActual", adCurrency, adParamInput,, PriceActual)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceLanded", adCurrency, adParamInput,, PriceLanded)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSimulated", adCurrency, adParamInput,, PriceSimulated)		'5
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertised", adCurrency, adParamInput,, PriceAdvertised)	'6
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSuggested", adCurrency, adParamInput,, PriceSuggested)		'7
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceOverride", adCurrency, adParamInput,, PriceOverride)		'8
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductPriceID", adInteger, adParamOutput)						'9
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = Cmd.Parameters(9).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddProductPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a products pricing.
'		Error True/False

Function DeleteProductPricing(ProductID)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products pricing.
'		Error True/False

Function UpdateProductPricing(ProductID, CompanyID, PriceActual, PriceLanded, PriceSimulated, PriceAdvertised, PriceSuggested, PriceOverride)
	Dim returnValue, Cmd
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_UpdateProductPricing")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceActual", adCurrency, adParamInput,, PriceActual)			'3
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceLanded", adCurrency, adParamInput,, PriceLanded)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSimulated", adCurrency, adParamInput,, PriceSimulated)		'5
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertised", adCurrency, adParamInput,, PriceAdvertised)	'6
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSuggested", adCurrency, adParamInput,, PriceSuggested)		'7
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceOverride", adCurrency, adParamInput,, PriceOverride)		'8
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	UpdateProductPricing = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products actual price.
'		Error True/False

Function SetProductPriceActual(ProductID, PriceActual, CompanyID)
	Dim returnValue
	returnValue = True
	If (Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) and _
		Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(Len(PriceActual) > 0)) Then PriceActual = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPriceActual")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceActual", adCurrency, adParamInput,, PriceActual)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPriceActual = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products landed price.
'		Error True/False

Function SetProductPriceLanded(ProductID, PriceLanded, CompanyID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) and _
		Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(Len(PriceLanded) > 0)) Then PriceLanded = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPriceLanded")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceLanded", adCurrency, adParamInput,, PriceLanded)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPriceLanded = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products simulated price.
'		Error True/False

Function SetProductPriceSimulated(ProductID, PriceSimulated, CompanyID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) and _
		Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(Len(PriceSimulated) > 0)) Then PriceSimulated = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPriceSimulated")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSimulated", adCurrency, adParamInput,, PriceSimulated)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3		
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPriceSimulated = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products advertised price.
'		Error True/False

Function SetProductPriceAdvertised(ProductID, PriceAdvertised, CompanyID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) and _
		Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(Len(PriceAdvertised) > 0)) Then PriceAdvertised = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPriceAdvertised")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertised", adCurrency, adParamInput,, PriceAdvertised)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPriceAdvertised = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products suggested price.
'		Error True/False

Function SetProductPriceSuggested(ProductID, PriceSuggested, CompanyID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) and _
		Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(Len(PriceSuggested) > 0)) Then PriceSuggested = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPriceSuggested")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSuggested", adCurrency, adParamInput,, PriceSuggested)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPriceSuggested = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Updates a products override price.
'		Error True/False

Function SetProductPriceOverride(ProductID, PriceOverride, CompanyID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID) and _
		Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(Len(PriceOverride) > 0)) Then PriceOverride = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_SetProductPriceOverride")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceOverride", adCurrency, adParamInput,, PriceOverride)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	SetProductPriceOverride = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a products lease price.
'		Returns ProductPriceLease/Null

Const prFairMarketLease			= 0
Const prOneDollarBuyoutLease	= 1
Const pr24MonthLease			= 0
Const pr36MonthLease			= 1
Const pr12MonthLease			= 2
Const pr48MonthLease			= 3

Function GetProductPriceLease(Price, LeaseType, LeaseLength)
	Dim returnValue, Multiplier, Rates(3,3), InternalPrice
	returnValue = Null
	If(IsNumeric(Price)) Then
		InternalPrice = CCur(Price)
		If(InternalPrice >= CCur(1000)) Then
			If (InternalPrice >= CCur(1000)) and (InternalPrice <= CCur(25000)) Then
				Rates(prFairMarketLease, pr12MonthLease) = .07734
				Rates(prFairMarketLease, pr24MonthLease) = .04148
				Rates(prFairMarketLease, pr36MonthLease) = .03005
				'Rates(prFairMarketLease, pr48MonthLease) = .00

				Rates(prOneDollarBuyoutLease, pr12MonthLease) = 0.09442		'12 month $1 buyout
				Rates(prOneDollarBuyoutLease, pr24MonthLease) = 0.04880		'24 month $1 buyout
				Rates(prOneDollarBuyoutLease, pr36MonthLease) = 0.03375		'36 month $1 buyout
				Rates(prOneDollarBuyoutLease, pr48MonthLease) = 0.02633		'48 month $1 buyout
			End If
			Multiplier = Rates(LeaseType, LeaseLength)
			returnValue = InternalPrice * Multiplier
		End If
	End If
	GetProductPriceLease = returnValue
End Function




'***************************************************************
'	DESCRIPTION:
'		Returns inventory across all distribution centers
'		for a particular product.  You can also specify
'		inventory that is only involved in order fulfillment.
'		i.e. stock that is available for sale.
'		Array/Null

Const prProductInventoryTotalProductID					= 0
Const prProductInventoryTotalQuantityOnHand				= 1
Const prProductInventoryTotalQuantityOrdered			= 2
Const prProductInventoryTotalQuantityBackordered		= 3
Const prProductInventoryTotalQuantityPicking			= 4
Const prProductInventoryTotalQuantitySold				= 5
Const prProductInventoryTotalQuantityAvailable			= 6

Function GetProductInventoryTotals(ProductID, OrderFulfillment)
	Dim returnValue, ary(6)
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(OrderFulfillment)) and Not(IsNull(OrderFulfillment)) and IsNumeric(OrderFulfillment)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductInventoryTotals")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrderFulfillment", adBoolean, adParamInput,, OrderFulfillment)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@QuantityOnHand", adInteger, adParamOutput)								'3
			Cmd.Parameters.Append Cmd.CreateParameter("@QuantityOrdered", adInteger, adParamOutput)								'4
			Cmd.Parameters.Append Cmd.CreateParameter("@QuantityBackordered", adInteger, adParamOutput)							'5
			Cmd.Parameters.Append Cmd.CreateParameter("@QuantityPicking", adInteger, adParamOutput)								'6
			Cmd.Parameters.Append Cmd.CreateParameter("@QuantitySold", adInteger, adParamOutput)								'7
			Cmd.Parameters.Append Cmd.CreateParameter("@QuantityAvailable", adInteger, adParamOutput)							'8
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				ary(prProductInventoryTotalProductID)			= ProductID
				ary(prProductInventoryTotalQuantityOnHand)		= Cmd.Parameters(3).Value
				ary(prProductInventoryTotalQuantityOrdered)		= Cmd.Parameters(4).Value
				ary(prProductInventoryTotalQuantityBackordered)	= Cmd.Parameters(5).Value
				ary(prProductInventoryTotalQuantityPicking)		= Cmd.Parameters(6).Value
				ary(prProductInventoryTotalQuantitySold)		= Cmd.Parameters(7).Value
				ary(prProductInventoryTotalQuantityAvailable)	= Cmd.Parameters(8).Value
				returnValue = ary
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	GetProductInventoryTotals = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns stock statuses for a particular product.
'		Returns Array/Null

Const prProductStockStatusesProductID = 0
Const prProductStockStatusesProductKey = 1
Const prProductStockStatusesProductCode = 2
Const prProductStockStatusesDateSellStart = 3
Const prProductStockStatusesCDWQuantityAvailable = 4
Const prProductStockStatusesCDWAverageReplenishmentLeadTime = 5
Const prProductStockStatusesDistributorQuantityAvailable = 6
Const prProductStockStatusesDistributorAverageReplenishmentLeadTime = 7
Const prProductStockStatusesCompanyID = 8
Const prProductStockStatusesQuantityBackordered = 9

Function GetProductStockStatuses(ProductID, CompanyID)
	Dim returnValue, ary(9)
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductStockStatuses")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)						'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductKey", adChar, adParamOutput, 32)									'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamOutput, 15)								'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamOutput)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@CDWQuantityAvailable", adInteger, adParamOutput)						'6
		Cmd.Parameters.Append Cmd.CreateParameter("@CDWAverageReplenishmentLeadTime", adInteger, adParamOutput)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@DistributorQuantityAvailable", adInteger, adParamOutput)				'8
		Cmd.Parameters.Append Cmd.CreateParameter("@DistributorAverageReplenishmentLeadTime", adInteger, adParamOutput)		'9
		Cmd.Parameters.Append Cmd.CreateParameter("@QuantityBackordered", adInteger, adParamOutput)							'10
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductStockStatusesProductID) = ProductID
			ary(prProductStockStatusesProductKey) = Cmd.Parameters(3).Value
			ary(prProductStockStatusesProductCode) = Cmd.Parameters(4).Value
			ary(prProductStockStatusesDateSellStart) = Cmd.Parameters(5).Value
			ary(prProductStockStatusesCDWQuantityAvailable)	= Cmd.Parameters(6).Value
			ary(prProductStockStatusesCDWAverageReplenishmentLeadTime) = Cmd.Parameters(7).Value
			ary(prProductStockStatusesDistributorQuantityAvailable)	= Cmd.Parameters(8).Value
			ary(prProductStockStatusesDistributorAverageReplenishmentLeadTime) = Cmd.Parameters(9).Value
			ary(prProductStockStatusesCompanyID) = CompanyID
			ary(prProductStockStatusesQuantityBackordered) = Cmd.Parameters(10).Value

			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductStockStatuses = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns stock statuses for multiple products. Pass a 1x
'		array, 2x array, or recordset with a column index for
'		the later two specifing ProductID's.  Pass a string
'		when you wish to build the where clause yourself.
'		Returns Array/Null

'	Note: Constants defined above

' Order by constants
Const prProductStockStatusOrderByProductID			= "p.ProductID"
Const prProductStockStatusOrderByProductCode		= "p.ProductCode"
Const prProductStockStatusOrderByFriendlyName		= "p.FriendlyName"
Const prProductStockStatusOrderByPriceAdvertised	= "p.PriceAdvertised"

Function GetProductStockStatusesBulk(ProductIDs, ProductIDIndex, ProductOrderBy, CompanyID)
	Dim returnValue, resultSet, TestDimension, ProductIDList, i
	returnValue = Null
	If(Not(IsEmpty(ProductIDs)) and Not(IsNull(ProductIDs))) Then
		If(Not(IsEmpty(ProductIDIndex)) and (IsNull(ProductIDIndex) or IsNumeric(ProductIDIndex))) Then
			If(Not(Len(ProductOrderBy) > 0)) Then ProductOrderBy = Null End If
			If(IsArray(ProductIDs)) Then
				If(Not(Is2xArray(ProductIDs))) Then
					' 1x Array
					ProductIDList = Join(ProductIDs, ",")
					ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
				Else
					' 2x Array
					ProductIDList = ""
					For i = Lbound(ProductIDs, 2) To Ubound(ProductIDs, 2)
						If(i = Ubound(ProductIDs, 2)) Then
							ProductIDList = ProductIDList & ProductIDs(ProductIDIndex, i)
						Else
							ProductIDList = ProductIDList & ProductIDs(ProductIDIndex, i) & ","
						End If
					Next
					ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
				End If
			ElseIf(IsObject(ProductIDs)) Then
				' RecordSet
				If(ProductIDs.State = adstateOpen) Then
					ProductIDs.MoveFirst
					ProductIDList = ""
					While(Not(ProductIDs.EOF))
						ProductIDList = ProductIDList & ProductIDs(ProductIDIndex) & ","
						ProductIDs.MoveNext
					Wend
					ProductIDList = Left(ProductIDList, Len(ProductIDList) - 1)
					ProductIDs.MoveFirst
					ProductIDList = "p.ProductID IN(" & ProductIDList & ")"
				End If
			ElseIf(VarType(ProductIDs) = vbString) Then
				' String
				ProductIDList = ProductIDs
			End If
			If(Not(IsEmpty(ProductIDList)) and Not(IsNull(ProductIDList)) and (Len(ProductIDList) > 0)) Then
				Dim Cmd
				Call OpenSQLConnection(ConnectionType_Product)
				Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductStockStatusesBulk2")
				Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, Null)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 4000, ProductIDList)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, ProductOrderBy)	'3
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)
				Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
				If(resultSet.State = adstateOpen) Then
					If(Not(resultSet.EOF)) Then
						returnValue = resultSet.GetRows
					End If
					resultSet.Close
				End If
				Set resultSet = Nothing
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Product)
			End If
		End If
	End If
	GetProductStockStatusesBulk = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns stock statuses for a particular query.  Join,
'		where, and order by clauses are exposed for greater
'		flexibility.
'		Returns Array/Null

Function GetProductStockStatusesAdHoc(JoinClause, WhereClause, OrderByClause, CompanyID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(WhereClause)) and Not(IsNull(WhereClause)) and Len(WhereClause) > 0) Then
		If(Not(Len(JoinClause) > 0)) Then JoinClause = Null End If
		If(Not(Len(OrderByClause) > 0)) Then OrderByClause = Null End If
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductStockStatusesBulk2")
		Cmd.Parameters.Append Cmd.CreateParameter("@JoinClause", adVarChar, adParamInput, 1000, JoinClause)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@WhereClause", adVarChar, adParamInput, 1000, WhereClause)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@OrderByClause", adVarChar, adParamInput, 100, OrderByClause)		'3
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductStockStatusesAdHoc = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns stock status for a particular product.
'		Returns Array/Null

Const prProductStockStatusName					= 0
Const prProductStockStatusDescription			= 1

Function GetProductStockStatus(DateSellStart, CDWQuantityAvailable, CDWAverageReplenishmentLeadTime, DistributorQuantityAvailable, DistributorAverageReplenishmentLeadTime, CompanyID, QuantityBackordered)
	Dim returnValue, ary(1), QuantityAvailable
	returnValue = Null

	'default if none of the if assignments below overwrite it.
	ary(prProductStockStatusName)			= prStockStatusNameTwoOrMoreWeeks & " "
	ary(prProductStockStatusDescription)	= prStockStatusDescriptionTwoOrMoreWeeks & " "

	' Instock
	'CDWQuantityAvailable = 0
	'Response.write "Here...." & CStr(CDWQuantityAvailable)
	'Canada specific, b/c drop ship
	If ( glWebSiteCompany = coCdwca ) Then
		If(DistributorQuantityAvailable > 0) Then
			ary(prProductStockStatusName)		= prStockStatusNameIn
			ary(prProductStockStatusDescription)	= prStockStatusDescriptionIn
		Else
			ary(prProductStockStatusName)		= prStockStatusNameThreeToSixDays
			ary(prProductStockStatusDescription)	= prStockStatusDescriptionThreeToSixDays
		End If
	Else 'Us market
		'Check if the item has Credit Approved Backorders.
		If QuantityBackordered > 0 Then
			QuantityAvailable = CDWQuantityAvailable - QuantityBackordered
		Else
			QuantityAvailable = CDWQuantityAvailable
		End If

		If(QuantityAvailable > 0) Then
			ary(prProductStockStatusName)			= prStockStatusNameIn
			ary(prProductStockStatusDescription)	= prStockStatusDescriptionIn
		Else 'not instock
			' Easily obtainable, next day or so. 5 is used instead of 0 to account for phantom stock that
			' disappears inbetween the time the order is placed and actually filled
			If(DistributorQuantityAvailable > 5) Then
				ary(prProductStockStatusName)			= prStockStatusNameOneToThreeDays
				ary(prProductStockStatusDescription)	= prStockStatusDescriptionOneToThreeDays
			ElseIf(Not(IsNull(DistributorAverageReplenishmentLeadTime))) Then
				'It's a little more involved getting this product
				' 10/04/2005 Andre Coetzee replaced 4-6 with 3-6 to sync with Product B/O
				If(DistributorAverageReplenishmentLeadTime <= 6) Then
					ary(prProductStockStatusName)			= prStockStatusNameThreeToSixDays
					ary(prProductStockStatusDescription)	= prStockStatusDescriptionThreeToSixDays
				ElseIf(DistributorAverageReplenishmentLeadTime <= 14) Then
					ary(prProductStockStatusName)			= prStockStatusNameOneToTwoWeeks
					ary(prProductStockStatusDescription)	= prStockStatusDescriptionOneToTwoWeeks
				Else
					ary(prProductStockStatusName)			= prStockStatusNameTwoOrMoreWeeks
					ary(prProductStockStatusDescription)	= prStockStatusDescriptionTwoOrMoreWeeks
				End If
			Else 'not instock, and item will not be available for 90 days
				If(DateDiff("d", Now(), DateSellStart) > -90) Then
					ary(prProductStockStatusName)			= prStockStatusNamePreOrder
					ary(prProductStockStatusDescription)	= prStockStatusDescriptionPreOrder
				End If
			End If
		End If
	End If
	returnValue = ary
	GetProductStockStatus = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a logical catalog.
'		Returns LogicalCatalogID/Null

Function AddLogicalCatalog(Name, Description)
	Dim returnValue
	returnValue = Null
	If Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0 Then
		If Not(Len(Description) > 0) Then Description = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddLogicalCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamOutput)				'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddLogicalCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a logical catalog.
'		Error True/False

Function DeleteLogicalCatalog(LogicalCatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteLogicalCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteLogicalCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a logical catalog.
'		Array/Null

Const prLogicalCatalogID			= 0
Const prLogicalCatalogKey			= 1
Const prLogicalCatalogName			= 2
Const prLogicalCatalogDescription	= 3
Const prLogicalCatalogDateCreated	= 4
Const prLogicalCatalogDateModified	= 5

Function GetLogicalCatalog(LogicalCatalogID)
	Dim returnValue, ary(5)
	returnValue = Null
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetLogicalCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogKey", adChar, adParamOutput, 32)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)								'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)						'6
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(prLogicalCatalogID)					= LogicalCatalogID
			ary(prLogicalCatalogKey)				= Cmd.Parameters(2).Value
			ary(prLogicalCatalogName)				= Cmd.Parameters(3).Value
			ary(prLogicalCatalogDescription)		= Cmd.Parameters(4).Value
			ary(prLogicalCatalogDateCreated)		= Cmd.Parameters(5).Value
			ary(prLogicalCatalogDateModified)		= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetLogicalCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all logical catalogs.
'		Array/Null

Function GetLogicalCatalogs()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetLogicalCatalogs")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Product)
	GetLogicalCatalogs = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a LogicalCatalogID from the guid.
'		Returns LogicalCatalogID/Null

Function GetLogicalCatalogID(LogicalCatalogKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(LogicalCatalogKey)) and Not(IsNull(LogicalCatalogKey)) and Len(LogicalCatalogKey) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetLogicalCatalogID")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogKey", adChar, adParamInput, 32, LogicalCatalogKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetLogicalCatalogID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product exists in the logical catalog.
'		Returns True/False

Function LogicalCatalogHasProduct(LogicalCatalogID, ProductID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_LogicalCatalogHasProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Product", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	LogicalCatalogHasProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a manufacture exists in the logical catalog.
'		Returns True/False

Function LogicalCatalogHasManufacture(LogicalCatalogID, ManufactureID)
	Dim returnValue, Cmd
	returnValue = False
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_LogicalCatalogHasManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Manufacture", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	LogicalCatalogHasManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product exists in the logical catalog.
'		Returns True/False

Function UserProductPurchasable(UserID, ProductID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_UserProductPurchasable")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Product", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False) 
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UserProductPurchasable = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a schema exists under a logical catalog
'		Returns True/False

Function LogicalCatalogHasSchema(LogicalCatalogID, SchemaID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(SchemaID)) and Not(IsNull(SchemaID)) and IsNumeric(SchemaID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_LogicalCatalogHasSchema")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@SchemaID", adInteger, adParamInput,, SchemaID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Schema", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	LogicalCatalogHasSchema = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns a product from a logical catalog.
'		Array/Null

'	Constants defined in GetProduct

Function GetLogicalCatalogProduct(LogicalCatalogID, ProductID)
	Dim returnValue, ary(22), Par
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetLogicalCatalogProduct")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductKey", adChar, adParamOutput, 32)								'4
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamOutput, 15)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)								'6
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyName", adVarChar, adParamOutput, 150)						'7
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)						'8
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyDescription", adVarChar, adParamOutput, 500)				'9
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyPartNumber", adVarChar, adParamOutput, 50)					'10
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)							'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureKey", adChar, adParamOutput, 32)							'12
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamOutput, 15)						'13
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureName", adVarChar, adParamOutput, 75)						'14
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturePartNumber", adVarChar, adParamOutput, 50)				'15
		Set Par = Cmd.CreateParameter("@Weight", adNumeric, adParamOutput)												'16
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Height", adNumeric, adParamOutput)												'17
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Width", adNumeric, adParamOutput)												'18
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Length", adNumeric, adParamOutput)												'19
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Cmd.Parameters.Append Cmd.CreateParameter("@NonReturnable", adBoolean, adParamOutput)							'20
		Cmd.Parameters.Append Cmd.CreateParameter("@DropShipOnly", adInteger, adParamOutput)							'21
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamOutput)						'22
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStop", adDBTimeStamp, adParamOutput)						'23
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)							'24
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)						'25
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductID)					= ProductID
			ary(prProductKey)					= Cmd.Parameters(3).Value
			ary(prProductCode)					= Cmd.Parameters(4).Value
			ary(prProductName)					= Cmd.Parameters(5).Value
			ary(prProductFriendlyName)			= Cmd.Parameters(6).Value
			ary(prProductDescription)			= Cmd.Parameters(7).Value
			ary(prProductFriendlyDescription)	= Cmd.Parameters(8).Value
			ary(prProductCompanyPartNumber)		= Cmd.Parameters(9).Value
			ary(prProductManufactureID)			= Cmd.Parameters(10).Value
			ary(prProductManufactureKey)		= Cmd.Parameters(11).Value
			ary(prProductManufactureCode)		= Cmd.Parameters(12).Value
			ary(prProductManufactureName)		= Cmd.Parameters(13).Value
			ary(prProductManufacturePartNumber)	= Cmd.Parameters(14).Value
			ary(prProductWeight)				= Cmd.Parameters(15).Value
			ary(prProductHeight)				= Cmd.Parameters(16).Value
			ary(prProductWidth)					= Cmd.Parameters(17).Value
			ary(prProductLength)				= Cmd.Parameters(18).Value
			ary(prProductNonReturnable)			= Cmd.Parameters(19).Value
			ary(prProductDropShipOnly)			= Cmd.Parameters(20).Value
			ary(prProductDateSellStart)			= Cmd.Parameters(21).Value
			ary(prProductDateSellStop)			= Cmd.Parameters(22).Value
			ary(prProductDateCreated)			= Cmd.Parameters(23).Value
			ary(prProductDateModified)			= Cmd.Parameters(24).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetLogicalCatalogProduct = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Gets all products available in the logical catalog.
'		RecordSet (to big for an array)

'	Constants defined in GetProduct

Function GetCatalogProducts(CatalogID)
	Dim Cmd, resultSet
		
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)	'1	
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
	Call CloseSQLConnection(ConnectionType_Product)
	Set GetCatalogProducts = resultSet
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all products in the current folder and
'		logical catalog.
'		Array/Null

'	Constants defined in GetProductFolderAssignments

Function GetLogicalCatalogProductFolderAssignments(LogicalCatalogID, CompanyID, FolderID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
			If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
				Call OpenSQLConnection(ConnectionType_Product)
				Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetLogicalCatalogProductFolderAssignments")
				Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)				'2
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'3
				Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
				If(resultSet.State = adstateOpen) Then
					If(Not(resultSet.EOF)) Then
						returnValue = resultSet.GetRows
					End If
					resultSet.Close
				End If
				Set resultSet = Nothing
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Product)
			End If
		End If
	End If
	GetLogicalCatalogProductFolderAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns product accessories in the current logical catalog.
'		Array/Null

'	Constants defined in GetProductAccessories

Function GetLogicalCatalogProductAccessories(LogicalCatalogID, ProductID)
	Dim returnValue, resultSet
	returnValue = Null
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetLogicalCatalogProductAccessories")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)				'2
			Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
			If(resultSet.State = adstateOpen) Then
				If(Not(resultSet.EOF)) Then
					returnValue = resultSet.GetRows
				End If
				resultSet.Close
			End If
			Set resultSet = Nothing
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	GetLogicalCatalogProductAccessories = returnValue
End Function


'***************************************************************
'	DESCRIPTION:	
'		Adds a catalog schema.
'		Returns CatalogSchemaID/Null

Function AddCatalogSchema(Name, Description)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogSchema")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamOutput)					'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddCatalogSchema = returnValue
End Function


Function AddLogicalCatalogSchemaAssignment(LogicalCatalogID, CatalogSchemaID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddLogicalCatalogSchemaAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,,LogicalCatalogID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogCatalogSchemaAssignmentID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddLogicalCatalogSchemaAssignment = returnValue
End Function

Function RemoveLogicalCatalogSchemaAssignment(LogicalCatalogID, CatalogSchemaID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_RemoveLogicalCatalogSchemaAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,,LogicalCatalogID)					'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	RemoveLogicalCatalogSchemaAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a catalog schema.
'		Error True/False

Function DeleteCatalogSchema(CatalogSchemaID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogSchema")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCatalogSchema = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a catalog schema.
'		Array/Null

Const prCatalogSchemaID				= 0
Const prCatalogSchemaKey			= 1
Const prCatalogSchemaName			= 2
Const prCatalogSchemaDescription	= 3
Const prCatalogSchemaDateCreated	= 4
Const prCatalogSchemaDateModified	= 5

Function GetCatalogSchema(CatalogSchemaID)
	Dim returnValue, ary(5)
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogSchema")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaKey", adChar, adParamOutput, 32)						'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)								'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)						'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)							'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)						'6
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(prCatalogSchemaID)				= CatalogSchemaID
			ary(prCatalogSchemaKey)				= Cmd.Parameters(2).Value
			ary(prCatalogSchemaName)			= Cmd.Parameters(3).Value
			ary(prCatalogSchemaDescription)		= Cmd.Parameters(4).Value
			ary(prCatalogSchemaDateCreated)		= Cmd.Parameters(5).Value
			ary(prCatalogSchemaDateModified)	= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogSchema = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all catalog schemas.
'		Array/Null

Function GetCatalogSchemas()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogSchemas")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Product)
	GetCatalogSchemas = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a CatalogSchemaID from the guid.
'		Returns CatalogSchemaID/Null

Function GetCatalogSchemaID(CatalogSchemaKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaKey)) and Not(IsNull(CatalogSchemaKey)) and Len(CatalogSchemaKey) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogSchemaID")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaKey", adChar, adParamInput, 32, CatalogSchemaKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogSchemaID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a CatalogSchemaID from the catalogID.
'		Returns CatalogSchemaID/Null

Function GetCatalogSchemaIDFromLogicalCatalogID(LogicalCatalogID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and isNumeric(LogicalCatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogSchemaIDByLogicalCatalogID")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogSchemaIDFromLogicalCatalogID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a manufacture to a catalog schema.
'		Returns CatalogIncludeManufactureID/Null

Function AddCatalogIncludeManufacture(CatalogSchemaID, ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogIncludeManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogIncludeManufactureID", adInteger, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogIncludeManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		returns catalogs assignmed to a catalog schema.
'		Returns catalogIDs/Null

Function GetCatalogIncludeManufactures(CatalogSchemaID)
	Dim returnValue
	Dim ResultSet
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogIncludeManufacture")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogIncludeManufactures = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		returns catalogs assigned to a catalog schema.
'		Returns catalogIDs/Null

Function GetCatalogExcludeManufactures(CatalogSchemaID)
	Dim returnValue
	Dim ResultSet
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogExcludeManufacture")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogExcludeManufactures = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		returns catalogs assignmed to a catalog schema.
'		Returns catalogIDs/Null

Function GetCatalogIncludeCatalogs(CatalogSchemaID)
	Dim returnValue, ResultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogIncludeCatalogs")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogIncludeCatalogs = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		returns catalogs assignmed to a catalog schema.
'		Returns catalogIDs/Null

Function GetCatalogExcludeCatalogs(CatalogSchemaID)
	Dim returnValue
	Dim ResultSet
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogExcludeCatalogs")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
		Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
		If(resultSet.State = adstateOpen) Then
			If(Not(resultSet.EOF)) Then
				returnValue = resultSet.GetRows
			End If
			resultSet.Close
		End If
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogExcludeCatalogs = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Adds a manufacture to a catalog schema.
'		Returns CatalogExcludeManufactureID/Null

Function AddCatalogExcludeManufacture(CatalogSchemaID, ManufactureID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogExcludeManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogExcludeManufactureID", adInteger, adParamOutput)			'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogExcludeManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a manufacture from a catalog schema.
'		Error True/False

Function DeleteCatalogIncludeManufacture(CatalogSchemaID, ManufactureID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogIncludeManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogIncludeManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a manufacture from a catalog schema.
'		Error True/False

Function DeleteCatalogExcludeManufacture(CatalogSchemaID, ManufactureID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogExcludeManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogExcludeManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a manufacture is included in the
'		catalog schema.
'		Returns True/False

Function CatalogSchemaHasIncludeManufacture(CatalogSchemaID, ManufactureID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogSchemaHasIncludeManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Manufacture", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogSchemaHasIncludeManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a manufacture is excluded in the
'		catalog schema.
'		Returns True/False

Function CatalogSchemaHasExcludeManufacture(CatalogSchemaID, ManufactureID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(ManufactureID)) and Not(IsNull(ManufactureID)) and IsNumeric(ManufactureID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogSchemaHasExcludeManufacture")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamInput,, ManufactureID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Manufacture", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogSchemaHasExcludeManufacture = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a folder to a catalog schema.
'		Returns CatalogIncludeFolderID/Null

Function AddCatalogIncludeFolder(CatalogSchemaID, FolderID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogIncludeFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogIncludeFolderID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogIncludeFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a folder to a catalog schema.
'		Returns CatalogExcludeFolderID/Null

Function AddCatalogExcludeFolder(CatalogSchemaID, FolderID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogExcludeFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogExcludeFolderID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogExcludeFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a folder from a catalog schema.
'		Error True/False

Function DeleteCatalogIncludeFolder(CatalogSchemaID, FolderID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogIncludeFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogIncludeFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a folder from a catalog schema.
'		Error True/False

Function DeleteCatalogExcludeFolder(CatalogSchemaID, FolderID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogExcludeFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogExcludeFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a folder is included in the
'		catalog schema.
'		Returns True/False

Function CatalogSchemaHasIncludeFolder(CatalogSchemaID, FolderID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogSchemaHasIncludeFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Folder", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogSchemaHasIncludeFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a folder is excluded in the
'		catalog schema.
'		Returns True/False

Function CatalogSchemaHasExcludeFolder(CatalogSchemaID, FolderID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogSchemaHasExcludeFolder")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)					'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Folder", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogSchemaHasExcludeFolder = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a catalog to a catalog schema.
'		Returns CatalogIncludeCatalogID/Null

Function AddCatalogIncludeCatalog(CatalogSchemaID, CatalogID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogIncludeCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogIncludeCatalogID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogIncludeCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a catalog to a catalog schema.
'		Returns CatalogExcludeCatalogID/Null

Function AddCatalogExcludeCatalog(CatalogSchemaID, CatalogID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogExcludeCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogExcludeCatalogID", adInteger, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogExcludeCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a catalog from a catalog schema.
'		Error True/False

Function DeleteCatalogIncludeCatalog(CatalogSchemaID, CatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogIncludeCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogIncludeCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a catalog from a catalog schema.
'		Error True/False

Function DeleteCatalogExcludeCatalog(CatalogSchemaID, CatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogExcludeCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogExcludeCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a catalog is included in the
'		catalog schema.
'		Returns True/False

Function CatalogSchemaHasIncludeCatalog(CatalogSchemaID, CatalogID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogSchemaHasIncludeCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Catalog", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogSchemaHasIncludeCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a catalog is excluded in the
'		catalog schema.
'		Returns True/False

Function CatalogSchemaHasExcludeCatalog(CatalogSchemaID, CatalogID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogSchemaID)) and Not(IsNull(CatalogSchemaID)) and IsNumeric(CatalogSchemaID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogSchemaHasExcludeCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogSchemaID", adInteger, adParamInput,, CatalogSchemaID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Catalog", adBoolean, adParamOutput)								'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogSchemaHasExcludeCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a Catalog.
'		Returns CatalogID/Null

Function AddCatalog(Name, Description)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
		If(Not(Len(Description) > 0)) Then Description = Null End If
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)					'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)	'2
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamOutput)						'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(3).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	AddCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a Catalog.
'		Error True/False

Function DeleteCatalog(CatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCatalog = returnValue
End Function

'***************************************************************
'	DESCRIPTION: 
'		Deletes a Logical Catalog and all items below it (manufacuturer links, folder links, physical catalog links
'		physical catalog products
'		Error True/False

Function DeleteLogicaCatalog(LogicalCatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalog_KT")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteLogicaCatalog = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Updates a Catalog.
'		Error True/False

Function UpdateCatalog(CatalogID, Name, Description)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(Name)) and Not(IsNull(Name)) and Len(Name) > 0) Then
			If(Not(Len(Description) > 0)) Then Description = Null End If
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_UpdateCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 75, Name)
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 500, Description)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
					returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	UpdateCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a Catalog.
'		Array/Null

Const prCatalogID			= 0
Const prCatalogKey			= 1
Const prCatalogName			= 2
Const prCatalogDescription	= 3
Const prCatalogDateCreated	= 4
Const prCatalogDateModified	= 5

Function GetCatalog(CatalogID)
	Dim returnValue, ary(5)
	returnValue = Null
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogKey", adChar, adParamOutput, 32)					'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'4
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'6
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(prCatalogID)				= CatalogID
			ary(prCatalogKey)				= Cmd.Parameters(2).Value
			ary(prCatalogName)				= Cmd.Parameters(3).Value
			ary(prCatalogDescription)		= Cmd.Parameters(4).Value
			ary(prCatalogDateCreated)		= Cmd.Parameters(5).Value
			ary(prCatalogDateModified)		= Cmd.Parameters(6).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all Catalogs.
'		Array/Null

Function GetCatalogs()
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogs")
	Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If(Not(resultSet.EOF)) Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Product)
	GetCatalogs = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a CatalogID from the guid.
'		Returns CatalogID/Null

Function GetCatalogID(CatalogKey)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogKey)) and Not(IsNull(CatalogKey)) and Len(CatalogKey) > 0) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogID")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogKey", adChar, adParamInput, 32, CatalogKey)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product exists in the catalog.
'		Returns True/False

Function CatalogHasProduct(CatalogID, ProductID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CatalogHasProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Product", adBoolean, adParamOutput)					'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CatalogHasProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a product to a catalog.
'		Returns CatalogProductAssignmentID/Null

Function AddCatalogProductAssignment(CatalogID, ProductID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_AddCatalogProductAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)			'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogProductAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogProductAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product from a catalog.
'		Error True/False

Function DeleteCatalogProductAssignment(CatalogProductAssignmentID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogProductAssignmentID)) and Not(IsNull(CatalogProductAssignmentID)) and IsNumeric(CatalogProductAssignmentID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogProductAssignment")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogProductAssignmentID", adInteger, adParamInput,, CatalogProductAssignmentID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCatalogProductAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product from a catalog.
'		Error True/False

Function DeleteCatalogProductAssignmentByProductID(ProductID, CatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogProductAssignmentByProductID")
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteCatalogProductAssignmentByProductID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes a product from all catalogs.
'		Error True/False

Function DeleteCatalogProductAssignments(ProductID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteCatalogProductAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = False
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteCatalogProductAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns a product from a catalog.
'		Array/Null

'	Constants defined in GetProduct

Function GetCatalogProduct(CatalogID, ProductID)
	Dim returnValue, ary(22), Par
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogProduct")
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductKey", adChar, adParamOutput, 32)					'3
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductCode", adVarChar, adParamOutput, 15)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput, 75)					'5
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyName", adVarChar, adParamOutput, 150)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 500)			'7
		Cmd.Parameters.Append Cmd.CreateParameter("@FriendlyDescription", adVarChar, adParamOutput, 500)	'8
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyPartNumber", adVarChar, adParamOutput, 50)		'9
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureID", adInteger, adParamOutput)				'10
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureKey", adChar, adParamOutput, 32)				'11
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureCode", adVarChar, adParamOutput, 15)			'12
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufactureName", adVarChar, adParamOutput, 75)			'13
		Cmd.Parameters.Append Cmd.CreateParameter("@ManufacturePartNumber", adVarChar, adParamOutput, 50)	'14
		Set Par = Cmd.CreateParameter("@Weight", adNumeric, adParamOutput)									'15
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Height", adNumeric, adParamOutput)									'16
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Width", adNumeric, adParamOutput)									'17
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Set Par = Cmd.CreateParameter("@Length", adNumeric, adParamOutput)									'18
		Par.Precision = 19
		Par.NumericScale = 4
		Cmd.Parameters.Append Par
		Set Par = Nothing
		Cmd.Parameters.Append Cmd.CreateParameter("@NonReturnable", adBoolean, adParamOutput)				'19
		Cmd.Parameters.Append Cmd.CreateParameter("@DropShipOnly", adInteger, adParamOutput)				'20
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStart", adDBTimeStamp, adParamOutput)			'21
		Cmd.Parameters.Append Cmd.CreateParameter("@DateSellStop", adDBTimeStamp, adParamOutput)			'22
		Cmd.Parameters.Append Cmd.CreateParameter("@DateCreated", adDBTimeStamp, adParamOutput)				'23
		Cmd.Parameters.Append Cmd.CreateParameter("@DateModified", adDBTimeStamp, adParamOutput)			'24
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductID)					= ProductID
			ary(prProductKey)					= Cmd.Parameters(3).Value
			ary(prProductCode)					= Cmd.Parameters(4).Value
			ary(prProductName)					= Cmd.Parameters(5).Value
			ary(prProductFriendlyName)			= Cmd.Parameters(6).Value
			ary(prProductDescription)			= Cmd.Parameters(7).Value
			ary(prProductFriendlyDescription)	= Cmd.Parameters(8).Value
			ary(prProductCompanyPartNumber)		= Cmd.Parameters(9).Value
			ary(prProductManufactureID)			= Cmd.Parameters(10).Value
			ary(prProductManufactureKey)		= Cmd.Parameters(11).Value
			ary(prProductManufactureCode)		= Cmd.Parameters(12).Value
			ary(prProductManufactureName)		= Cmd.Parameters(13).Value
			ary(prProductManufacturePartNumber)	= Cmd.Parameters(14).Value
			ary(prProductWeight)				= Cmd.Parameters(15).Value
			ary(prProductHeight)				= Cmd.Parameters(16).Value
			ary(prProductWidth)					= Cmd.Parameters(17).Value
			ary(prProductLength)				= Cmd.Parameters(18).Value
			ary(prProductNonReturnable)			= Cmd.Parameters(19).Value
			ary(prProductDropShipOnly)			= Cmd.Parameters(20).Value
			ary(prProductDateSellStart)			= Cmd.Parameters(21).Value
			ary(prProductDateSellStop)			= Cmd.Parameters(22).Value
			ary(prProductDateCreated)			= Cmd.Parameters(23).Value
			ary(prProductDateModified)			= Cmd.Parameters(24).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetCatalogProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all products in the current folder and catalog.
'		Array/Null

'	Constants defined in GetProductFolderAssignments

Function GetCatalogProductFolderAssignments(CatalogID, CompanyID, FolderID)
	Dim returnValue, resultSet, Cmd
	returnValue = Null
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
			If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
				Call OpenSQLConnection(ConnectionType_Product)
				Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetCatalogProductFolderAssignments")
				Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)	'1
				Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)	'2
				Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)		'3
				Set resultSet = ExecuteSQLProcedure(ConnectionType_Product, Cmd, True)
				If(resultSet.State = adstateOpen) Then
					If(Not(resultSet.EOF)) Then
						returnValue = resultSet.GetRows
					End If
					resultSet.Close
				End If
				Set resultSet = Nothing
				Set Cmd = Nothing
				Call CloseSQLConnection(ConnectionType_Product)
			End If
		End If
	End If
	GetCatalogProductFolderAssignments = returnValue
End Function


























'***************************************************************
'	DESCRIPTION:
'		Adds a catalog to an organization
'		CatalogOrganizationAssignmentID/Null

Function AddCatalogToOrganization(CatalogID, OrganizationID)
	Dim returnValue
	returnValue = null
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddCatalogToOrganization")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogOrganizationAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogToOrganization = returnValue
End Function



'***************************************************************
'	DESCRIPTION:
'		Adds a catalog to a user
'		CatalogUserAssignmentID/Null
Function AddCatalogToUser(CatalogID, UserID)
	Dim returnValue
	returnValue = null
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddCatalogToUser")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, UserID)		'2
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogUserAssignmentID", adInteger, adParamOutput)		'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddCatalogToUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Removes a catalog From a user
'		true/false

Function RemoveCatalogFromUser(CatalogID, UserID)
	Dim returnValue
	returnValue = True	
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_RemoveCatalogFromUser")
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, CatalogID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, UserID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	RemoveCatalogFromUser = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Sets an organizations default catalog
'		true/false
Function SetDefaultOrganizationLogicalCatalog(CatalogID, OrganizationID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetDefaultOrganizationLogicalCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	SetDefaultOrganizationLogicalCatalog = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Gets all of the groups that a Catalog belongs to
'		Error True/False
Function GetCatalogGroups(LogicalCatalogID)
	Dim returnValue
	returnValue = null
	If Not(IsNull(LogicalCatalogID)) And IsNumeric(LogicalCatalogID) Then
		Dim Cmd
		Dim resultSet
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_GetCatalogGroups")
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID",adInteger,adParamInput,,LogicalCatalogID)			'1
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
	GetCatalogGroups = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Determines if a group has a LogicalCatalog
'		Error True/False

Function GroupHasCatalog(GroupID, LogicalCatalogID)
	dim returnValue
	returnValue = False
	
	If Not IsEmpty(GroupID) and Not IsNull(GroupID) and IsNumeric(GroupID) Then
		If Not IsEmpty(LogicalCatalogID) and Not IsNull(LogicalCatalogID) and IsNumeric(LogicalCatalogID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GroupHasCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@GroupID", adInteger, adParamInput,, GroupID)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID) '2
			Cmd.Parameters.Append Cmd.CreateParameter("@Status", adBoolean, adParamOutput)							'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	GroupHasCatalog = returnValue
End Function




'***************************************************************
'	DESCRIPTION:
'		Gets an organizations default catalog
'		Null/CatalogID

Function GetDefaultOrganizationLogicalCatalog(OrganizationID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetDefaultOrganizationLogicalCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamOutput)	'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetDefaultOrganizationLogicalCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		sets a users default catalog
'		true/false
Function SetDefaultUserLogicalCatalog(CatalogID, UserID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_SetDefaultUserLogicalCatalog")
			Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'2
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	
	SetDefaultUserLogicalCatalog = returnValue
End Function


'***************************************************************  
'	DESCRIPTION:
'		sets a users default catalog
'		true/false

Function GetDefaultUserLogicalCatalog(UserID)
	Dim returnValue
	returnValue = Null
	
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetDefaultUserLogicalCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamOutput)			'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetDefaultUserLogicalCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		returns a user's alerts
'		Null/array
Function GetUserAlerts(UserID)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserAlerts")
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
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
	
	GetUserAlerts = returnValue
End Function


Function GetUserAlert(UserID, ProductID, CatalogID, AlertTypeID)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			If(Not(IsEmpty(CatalogID)) and Not(IsNull(CatalogID)) and IsNumeric(CatalogID)) Then
				If(Not(IsEmpty(AlertTypeID)) and Not(IsNull(AlertTypeID)) and IsNumeric(AlertTypeID)) Then
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserAlert")
					Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'2
					Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,, CatalogID)		'3
					Cmd.Parameters.Append Cmd.CreateParameter("@AlertTypeID", adInteger, adParamInput,, AlertTypeID)	'4
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
			End If
		End If
	End If
	GetUserAlert = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		returns a user's alerts
'		null/array

'constants defined in GetUserAlerts

Function GetUserAlertsByAlertTypeID(UserID, AlertTypeID)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
		If(Not(IsEmpty(AlertTypeID)) and Not(IsNull(AlertTypeID)) and IsNumeric(AlertTypeID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetUserAlertsByAlertTypeID")
			Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AlertTypeID", adInteger, adParamInput,, UseAlertTypeIDrID)		'2
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
	End If
	GetUserAlertsByAlertTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds a alert to a user
'		AlertID/Null

Function AddUserAlert(UserID, ProductID, CatalogID, AlertTypeID, Amount)
	Dim returnValue
	returnValue = null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		If(Not(IsEmpty(UserID)) and Not(IsNull(UserID)) and IsNumeric(UserID)) Then
			If(Not(IsEmpty(AlertTypeID)) and Not(IsNull(AlertTypeID)) and IsNumeric(AlertTypeID)) Then
				Dim Cmd
				Call OpenSQLConnection(ctReadWrite)
				Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddUserAlert")
				Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'2
				Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,,CatalogID)			'3
				Cmd.Parameters.Append Cmd.CreateParameter("@AlertTypeID", adInteger, adParamInput,,AlertTypeID)		'4
				Cmd.Parameters.Append Cmd.CreateParameter("@Amount", adCurrency, adParamInput,,Amount)				'5
				Cmd.Parameters.Append Cmd.CreateParameter("@UserAlertID", adInteger, adParamOutput)					'6
				Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
				If(Cmd.Parameters(0).Value = 0) Then
					returnValue = Cmd.Parameters(6).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	End If
	AddUserAlert = returnValue
End Function


Function DeleteUserAlert(AlertID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(AlertID) and Not IsNull(AlertID) and IsNumeric(AlertID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteAlert")
		Cmd.Parameters.Append Cmd.CreateParameter("@AlertID", adInteger, adParamInput,,AlertID)				'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = True
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteUserAlert = returnValue
End Function


Function DeleteUserAlertByProductID(UserID, ProductID, CatalogID, AlertTypeID)
	Dim returnValue
	returnValue = False
	If Not IsEmpty(UserID) and Not IsNull(UserID) and IsNumeric(UserID) Then
		If Not IsEmpty(ProductID) and Not IsNull(ProductID) and IsNumeric(ProductID) Then
			If Not IsEmpty(CatalogID) and Not IsNull(CatalogID) and IsNumeric(CatalogID) Then
				If Not IsEmpty(AlertTypeID) and Not IsNull(AlertTypeID) and IsNumeric(AlertTypeID) Then
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteAlertByCriteria")
					Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,,UserID)				'1
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,,ProductID)			'2
					Cmd.Parameters.Append Cmd.CreateParameter("@CatalogID", adInteger, adParamInput,,CatalogID)			'3
					Cmd.Parameters.Append Cmd.CreateParameter("@AlertTypeID", adInteger, adParamInput,,AlertTypeID)		'4					
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If(Cmd.Parameters(0).Value = 0) Then
						returnValue = True
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	End If
	DeleteUserAlertByProductID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the BookmarkID for a key
Function GetAlertTypeID(AlertKey)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(AlertKey) and Not IsNull(AlertKey) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.CDW_GetAlertTypeID")
		Cmd.Parameters.Append Cmd.CreateParameter("@AlertKey", adVarChar, adParamInput,32,AlertKey)				'1
		Cmd.Parameters.Append Cmd.CreateParameter("@AlertTypeID", adInteger, adParamOutput)						'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetAlertTypeID = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns all In Stock Buyers Alerts
'		null/array

Const ba_is_UserAlertID				= 0
Const ba_is_UserID					= 1
Const ba_is_UserContactName			= 2
Const ba_is_EmailAddress			= 3
Const ba_is_ProductID				= 4
Const ba_is_ProductCode				= 5
Const ba_is_FriendlyName			= 6
Const ba_is_FriendlyDescription		= 7
Const ba_is_QuantityAvailable		= 8
Const ba_is_DateCreated				= 9

Function GetInStockBuyersAlerts()
	Dim returnValue
	Dim resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetInStockBuyersAlerts")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	GetInStockBuyersAlerts = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns all Price Change Buyers Alerts
'		null/array

Const ba_pc_UserAlertID				= 0
Const ba_pc_UserID					= 1
Const ba_pc_CompanyID				= 2
Const ba_pc_UserContactName			= 3
Const ba_pc_EmailAddress			= 4
Const ba_pc_ProductID				= 5
Const ba_pc_ProductCode				= 6
Const ba_pc_FriendlyName			= 7
Const ba_pc_FriendlyDescription		= 8
Const ba_pc_Amount					= 9
Const ba_pc_DateCreated				= 10

Function GetPriceChangeBuyersAlerts()
	Dim returnValue
	Dim resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetPriceChangeBuyersAlerts")
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If (resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ctReadWrite)
	GetPriceChangeBuyersAlerts = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Adds an assignment between a logical catalog and an organization
'		Error True/False

Function AddOrganizationLogicalCatalogAssignment(OrganizationID, LogicalCatalogID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_AddOrganizationLogicalCatalogAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationLogicalCatalogAssignmentID", adInteger, adParamOutput)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	AddOrganizationLogicalCatalogAssignment = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		gets all logical catalogs for an organization
'		Null/resultset
Const eaOrganizationLogicalCatalogID = 0

Function GetOrganizationLogicalCatalogAssignments(OrganizationID)
	Dim returnValue
	Dim resultSet
	returnValue = Null
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationLogicalCatalogAssignments")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
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
	GetOrganizationLogicalCatalogAssignments = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Deletes an assignment between a logical catalog and an organization
'		Error True/False

Function DeleteOrganizationLogicalCatalogAssignment(OrganizationID, LogicalCatalogID)
	Dim returnValue
	returnValue = True
	If(Not(IsEmpty(OrganizationID)) and Not(IsNull(OrganizationID)) and IsNumeric(OrganizationID)) Then
		If(Not(IsEmpty(LogicalCatalogID)) and Not(IsNull(LogicalCatalogID)) and IsNumeric(LogicalCatalogID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_DeleteOrganizationLogicalCatalogAssignment")
			Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)
			Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	End If
	DeleteOrganizationLogicalCatalogAssignment = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the the Logical Catalog for an Organization's Vendornet.
Function GetVendornetLogicalCatalog(OrganizationID)
	Dim returnValue
	returnValue = Null
	
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetVendorNetLogicalCatalog")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,,OrganizationID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@LogicalCatalogID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	
	GetVendornetLogicalCatalog = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets information about a bundle
'		Error Null/array

Const bdlBundleID				= 0
Const bdlBundleName				= 1
Const bdlBundleDescription		= 2

Function GetBundle(BundleID)
	Dim returnValue
	returnValue = Null
	If Not IsEmpty(BundleID) and Not IsNull(BundleID) and IsNumeric(BundleID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Bundle_GetBundle")
		Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamOutput,200)				'2
		Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamOutput, 1000)		'3
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If Cmd.Parameters(0).Value = 0 then
			Dim ary(2)
			ary(bdlBundleID)					= BundleID
			ary(bdlBundleName)					= Cmd.Parameters(2).Value
			ary(bdlBundleDescription)			= Cmd.Parameters(3).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetBundle = returnValue	
End Function


'***************************************************************
'	DESCRIPTION:
'		Gets products from a bundle
'		Error Null/array

Const bdlBundleItemID			= 0
Const bdlBundleItemProductID	= 1
Const bdlBundleItemQuantity		= 2

Function GetBundleItems(BundleID)
	Dim returnValue
	Dim ResultSet
	returnValue = Null
	If Not IsEmpty(BundleID) and Not IsNull(BundleID) and IsNumeric(BundleID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Bundle_GetBundleItems")
		Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)		'1
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
	End If
	GetBundleItems = returnValue	
End Function

Function AddBundle(Name, Description)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(Name) and Not IsNull(Name) and Len(Name) > 0 Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Bundle_AddBundle")
			Cmd.Parameters.Append Cmd.CreateParameter("@Name", adVarChar, adParamInput, 200, Name)				'1
			Cmd.Parameters.Append Cmd.CreateParameter("@Description", adVarChar, adParamInput, 1000, Description)'2
			Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamOutput)					'3
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = Cmd.Parameters(3).Value
			else
				Response.Write("error")
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	AddBundle = returnValue
End Function

Function AddBundleItem(BundleID, ProductID, Quantity)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(BundleID) and Not IsNull(BundleID) and IsNumeric(BundleID) Then
			If Not IsEmpty(ProductID) and Not IsNull(ProductID) and IsNumeric(ProductID) Then
				If Not IsEmpty(Quantity) and Not IsNull(Quantity) and IsNumeric(Quantity) Then
					Dim Cmd
					Call OpenSQLConnection(ctReadWrite)
					set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Bundle_AddBundleItem")
					Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)	'1
					Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)'2
					Cmd.Parameters.Append Cmd.CreateParameter("@Quantity", adInteger, adParamInput,, Quantity)'2
					Cmd.Parameters.Append Cmd.CreateParameter("@BundleItemID", adInteger, adParamOutput)		'3
					Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
					If Cmd.Parameters(0).Value = 0 then
						returnValue = Cmd.Parameters(3).Value
					End If
					Set Cmd = Nothing
					Call CloseSQLConnection(ctReadWrite)
				End If
			End If
		End If
	AddBundleItem = returnValue
End Function

Function GetOrganizationBundles(OrganizationID)
	Dim returnValue
	Dim ResultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationBundles")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
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
	End If
	GetOrganizationBundles = returnValue
End Function

Function GetOrganizationUserBundles(OrganizationID, UserID)
	Dim returnValue
	Dim ResultSet
	returnValue = Null
	If Not IsEmpty(OrganizationID) and Not IsNull(OrganizationID) and IsNumeric(OrganizationID) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Eaccount_GetOrganizationUserBundles")
		Cmd.Parameters.Append Cmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@UserID", adInteger, adParamInput,, UserID)		'2
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
	End If
	GetOrganizationUserBundles = returnValue
End Function

Function DeleteBundle(BundleID)
	Dim returnValue
	returnValue = True
		If Not IsEmpty(BundleID) and Not IsNull(BundleID) and IsNumeric(BundleID) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Bundle_DeleteBundle")
			Cmd.Parameters.Append Cmd.CreateParameter("@BundleID", adInteger, adParamInput,, BundleID)	'1
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If Cmd.Parameters(0).Value = 0 then
				returnValue = False
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		End If
	DeleteBundle = returnValue
End Function

Const iibQty = 0
Const iibEDC = 1
Const iibName = 2

Function GetCDWBundle(ProductCode)
	dim returnValue, cmd, resultSet
	returnValue = null
	if Not IsEmpty(ProductCode) and Not IsNull(ProductCode) and IsNumeric(ProductCode) then
		Call OpenSQLConnection(ctReadWrite)
		set cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_GetBundleItemsInfo")
		cmd.Parameters.Append cmd.CreateParameter("@EDC",adInteger,adParamInput,,ProductCode)
		Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
		if (resultSet.State = adstateOpen) then
			if (not resultSet.EOF) then
				returnValue = resultSet.getRows
			end if
			resultSet.Close
		end if
		Set resultSet = Nothing
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	end if
	GetCDWBundle = returnValue
end function


'***************************************************************
'	DESCRIPTION:
'		Returns a ProductID from the transactionItemID
'		Returns ProductID/Null

Function GetProductIDByTransactionItemID(transactionItemID)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(transactionItemID)) and Not(IsNull(transactionItemID)) and IsNumeric(transactionItemID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductIDbyTransactionItemID")
		Cmd.Parameters.Append Cmd.CreateParameter("@transactionItemID", adInteger, adParamInput,, transactionItemID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamOutput)				'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductIDByTransactionItemID = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Returns all current products bundles.
'		RecordSet/Null 


Function GetAllProductBundles()
	Dim Cmd, returnvalue, resultSet
	returnvalue=NULL
	Call OpenSQLConnection(ctReadWrite)
	Set Cmd = CreateStoredProcCommand("WebDB.dbo.Product_GetAllProductBundles")
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
	GetAllProductBundles = returnvalue
End Function

''************************************************************************************************************************************
function GetCrossSellItems(EDCs)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Rank_GetCrossSellEDCs")
	Cmd.Parameters.Append Cmd.CreateParameter("@EDCCommaDelimitedArray", adVarChar, adParamInput, 1000, EDCs)										
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Product)

	GetCrossSellItems = returnValue
end function

''************************************************************************************************************************************
function GetTopSellingItemsByFolderCode(FolderCode, NumberToReturn)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_GetFolderTopProducts")
	Cmd.Parameters.Append Cmd.CreateParameter("@FolderCode", adChar, adParamInput, 2, FolderCode)										
	Cmd.Parameters.Append Cmd.CreateParameter("@NumberToReturn", adInteger, adParamInput, , NumberToReturn)										
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Product)

	GetTopSellingItemsByFolderCode = returnValue
end function

'****************************************************************************************************************************************
' NOTE ctReadWrite should not be changed, even though this function could get data from the RO boxes, users 
' of attribute editor would change data and not see if right away. Very confusing.

const getAccessoryRankings_FolderID		= 0
const getAccessoryRankings_FolderName	= 1
const getAccessoryRankings_FolderCode	= 2

function getProductAccessoryRankings(FolderID)
	Dim returnValue, resultSet
	returnValue = Null
	Dim Cmd
	Call OpenSQLConnection(ConnectionType_Product)
	Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_GetProductAccessoryRankings")
	Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput, , FolderID)		'1								
	Set resultSet = ExecuteSQLProcedure(ctReadWrite, Cmd, True)
	If(resultSet.State = adstateOpen) Then
		If Not resultSet.EOF Then
			returnValue = resultSet.GetRows
		End If
		resultSet.Close
	End If
	Set resultSet = Nothing
	Set Cmd = Nothing
	Call CloseSQLConnection(ConnectionType_Product)

	getProductAccessoryRankings = returnValue
end function

'****************************************************************************************************************************************
' NOTE ctReadWrite should never be changed

function AddProductAccessoryRanking(FolderID, AccessoryFolderID, Rank)
	Dim returnValue
	returnValue = Null
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		If(Not(IsEmpty(AccessoryFolderID)) and Not(IsNull(AccessoryFolderID)) and IsNumeric(AccessoryFolderID)) Then
			Dim Cmd
			Call OpenSQLConnection(ctReadWrite)
			Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_AddProductAccessoryRanking")
			Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)						'1
			Cmd.Parameters.Append Cmd.CreateParameter("@AccessoryFolderID", adInteger, adParamInput,, AccessoryFolderID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Rank", adInteger, adParamInput,, Rank)								'3
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductAccessoryRankingID", adInteger, adParamOutput)				'4
			Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(4).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ctReadWrite)
		end if
	End If
	AddProductAccessoryRanking = returnValue
end function

'****************************************************************************************************************************************
' NOTE ctReadWrite should never be changed

' Returns true to indicate problems, else false

' Gets rid of all of the rankings, when users update these, it's an all or nothing action. 

function DeleteProductAccessoryRankings(FolderID)
	Dim returnValue
	returnValue = true 
	If(Not(IsEmpty(FolderID)) and Not(IsNull(FolderID)) and IsNumeric(FolderID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_DeleteProductAccessoryRankings")
		Cmd.Parameters.Append Cmd.CreateParameter("@FolderID", adInteger, adParamInput,, FolderID)						'1
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = false 'no problems
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	DeleteProductAccessoryRankings = returnValue
end function


'**************************************************************************************************************************************
' Description: Returns pricing information for a specified product (ProductID) and Company (CompanyID)
' Created By: David Tzau
' Date: Oct.25.2001
'****************************************************************************************************************************************
Const prProductPriceActual				= 0
Const prProductPriceLanded				= 1
Const prProductPriceSimulated			= 2
Const prProductPriceAdvertised			= 3
Const prProductPriceSuggested			= 4
Const prProductPriceOverride			= 5

Function GetProductPrices(ProductID, CompanyID)
	Dim returnValue, ary(6)  'return variables
		
	'initialize returnValue
	returnValue = Null	

	If (Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) and _
		(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID))Then
		'perform query only if ProductId is passed in.  Retrieve pricing info 
		Dim Cmd

		Call OpenSQLConnection(ConnectionType_Product)							   			
		
		'create ADODB.Command object
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductPrices")
		
		'create parameters for stored proc
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)		'2
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceActual", adCurrency, adParamOutput)				'3
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceLanded", adCurrency, adParamOutput)				'4
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSimulated", adCurrency, adParamOutput)				'5
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceAdvertised", adCurrency, adParamOutput)			'6
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceSuggested", adCurrency, adParamOutput)				'7
		Cmd.Parameters.Append Cmd.CreateParameter("@PriceOverride", adCurrency, adParamOutput)				'8	
		
		'execute the Command
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)

		If(Cmd.Parameters(0).Value = 0) Then		
			'serialize to array only if no errors
			ary(prProductPriceActual)			= Cmd.Parameters(3).Value
			ary(prProductPriceLanded)			= Cmd.Parameters(4).Value
			ary(prProductPriceSimulated)		= Cmd.Parameters(5).Value
			ary(prProductPriceAdvertised)		= Cmd.Parameters(6).Value
			ary(prProductPriceSuggested)		= Cmd.Parameters(7).Value
			ary(prProductPriceOverride)		= Cmd.Parameters(8).Value			
			returnValue = ary
		End If
		
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)
	End If
	GetProductPrices = returnValue
End Function

'***********************
' IsProductDropShipOnly(ProductID,ProductCode)
' Description: Will tell you if an item is drop ship only
' Used by the shipping.inc page
' Written by PM on 5/24/2002
' returns back either a True or False

' Note: It doesnt require you to send both ProductID or ProductCode.
' Send EITHER. Null the other value, or I will. The sproc was built to handle either

Function IsProductDropShipOnly(ProductID,ProductCode)

	Dim returnValue
	Dim oCmd		'piss off I like the Hungarians
	returnValue = null
	
	If IsNull(ProductID) or IsEmpty(ProductID) or Len(ProductID) <= 0 Then
		' Just be safe. 
		ProductID = Null
	End If
	
	If IsNull(ProductCode) or IsEmpty(ProductCode) or Len(ProductCode) <= 0 Then
		' Just be safe. 
		ProductCode = Null
	End If

	If Not(IsNull(ProductID)) or Not(IsNull(ProductCode)) Then
		Call OpenSQLConnection(ConnectionType_Product)
		Set oCmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_IsDropShipOnly")
		oCmd.Parameters.Append oCmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)			'1
		oCmd.Parameters.Append oCmd.CreateParameter("@ProductCode", adVarChar, adParamInput,15, ProductCode)	'2
		oCmd.Parameters.Append oCmd.CreateParameter("@IsDropShipOnly", adBoolean, adParamOutput)				'3
		Call ExecuteSQLProcedure(ConnectionType_Product, oCmd, False)
		If(oCmd.Parameters(0).Value = 0) Then
			returnValue = oCmd.Parameters(3).Value
		End If
	End If
	IsProductDropShipOnly = returnValue
End Function

Function OrganizationHasLogicalCatalog(LogicalCatalogID, OrganizationID)
	Dim ReturnValue
	Dim oCmd
	returnValue = False
	If Not(IsNull(LogicalCatalogID)) and Not(IsEmpty(LogicalCatalogID)) and IsNumeric(LogicalCatalogID) Then
		If Not(IsNull(OrganizationID)) and Not(IsEmpty(OrganizationID)) and IsNumeric(OrganizationID) Then
			Call OpenSQLConnection(ctReadWrite)
			Set oCmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_OrganizationHasLogicalCatalog")
			oCmd.Parameters.Append oCmd.CreateParameter("@LogicalCatalogID", adInteger, adParamInput,, LogicalCatalogID)			'1
			oCmd.Parameters.Append oCmd.CreateParameter("@OrganizationID", adInteger, adParamInput,, OrganizationID)	'2
			oCmd.Parameters.Append oCmd.CreateParameter("@HasCatalog", adBoolean, adParamOutput)				'3
			Call ExecuteSQLProcedure(ctReadWrite, oCmd, False)
			If(oCmd.Parameters(0).Value = 0) Then
				returnValue = oCmd.Parameters(3).Value
			End If
		End If
	End If
	OrganizationHasLogicalCatalog = returnValue
End Function
'***********************

Function GetCompanyCatalogSchema()
	Dim ReturnValue
	returnValue = prCDWCatalogSchema
	Select Case glWebSiteKey
		Case www_cdw_com
			ReturnValue = prCDWCatalogSchema
		Case www_cdwg_com
			ReturnValue = prCDWGCatalogSchema
		Case www_cdw_ca
			ReturnValue = prCDWCACatalogSchema
	End Select
	GetCompanyCatalogSchema = returnValue
End Function
'***********************
Function GetCompanyLogicalCatalog()
	Dim ReturnValue
	returnValue = prCDWLogicalCatalog
	Select Case glWebSiteKey
		Case www_cdw_com
			ReturnValue = prCDWLogicalCatalog
		Case www_cdwg_com
			ReturnValue = prCDWGLogicalCatalog
		Case www_cdw_ca
			ReturnValue = prCDWCALogicalCatalog
	End Select
	GetCompanyLogicalCatalog = returnValue
End Function
'***************************************************************
'	DESCRIPTION:
'		Checks whether or not a product exists for a specific company.
'		Returns True/False

Function CompanyHasProduct(CompanyID, ProductID)
	Dim returnValue
	returnValue = False
	If(Not(IsEmpty(CompanyID)) and Not(IsNull(CompanyID)) and IsNumeric(CompanyID)) Then
		If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
			Dim Cmd
			Call OpenSQLConnection(ConnectionType_Product)
			set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_CompanyHasProduct")
			Cmd.Parameters.Append Cmd.CreateParameter("@CompanyID", adInteger, adParamInput,, CompanyID)	'1
			Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'2
			Cmd.Parameters.Append Cmd.CreateParameter("@Product", adBoolean, adParamOutput)			'3
			Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
			If(Cmd.Parameters(0).Value = 0) Then
				returnValue = Cmd.Parameters(3).Value
			End If
			Set Cmd = Nothing
			Call CloseSQLConnection(ConnectionType_Product)
		End If
	End If
	CompanyHasProduct = returnValue
End Function


'***************************************************************
'	DESCRIPTION:
'		Returns the Product Group
'		Returns ProductGroup/NULL

Function GetProductGroup(ProductID)
	Dim returnValue
	returnValue = null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductGroup")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductGroup", adVarChar, adParamOutput, 15)		'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)	
	End If
	GetProductGroup = returnValue
End Function





'**********************************************************************
'	DESCRIPTION:
'		Returns a short stock status message. e.g. In Stock, Call
'	NOTES:
'		Makes an http webservice call.
'
Function GetShortStockStatusMessage(productId, companyId)
	dim uri
	const WAIT_TIME = 10
	dim returnXml
	dim nodes
	dim mydoc

	uri = "http://" + GetSiteName(webservice_cdw_com) + "/CDW/BusinessObjects/Product/ProductWebService.asmx/GetStockStatusShortMessage?productID=" & productId & "&companyId=" & companyId & "&messageToGet=1"
	returnXml = GetRemoteUriContents(uri, WAIT_TIME)

	Set mydoc = Server.CreateObject("Microsoft.XMLDOM")
	set nodes = Server.CreateObject("Microsoft.XMLDOM")

	'load xml into dom
	mydoc.loadXml returnXml

	'get node
	set nodes = mydoc.getElementsByTagName("string")

	If (nodes.Length = 0) Or IsEmpty(nodes.item(0)) OR IsNull(nodes.item(0)) then
		set mydoc = nothing
		set nodes = nothing
		GetShortStockStatusMessage = "Call"
	Else
		'parse out our string
		returnXml = nodes.item(0).text

		If Len(Trim(returnXml)) = 0 OR Trim(returnXml) = "" Then returnXml = "Call" End If

		set mydoc = nothing
		set nodes = nothing

		GetShortStockStatusMessage = returnXml
	End If
end function

'**********************************************************************
'	DESCRIPTION:
'		Returns the xml from a http web service call.
'	NOTES:
'		If we can't retrieve the message over http we'll return Call.  So 
'		if there is a page full of Call, we know there is a problem.
'
Function GetRemoteUriContents(Uri, Timeout)
	Dim objXmlHttp
	Dim strHTML
	dim error
	const READY = 4
	const CANT_GET = "Call" 

	Set objXmlHttp = Server.CreateObject("Msxml2.ServerXMLHTTP")

	''set to True for asynch
	objXmlHttp.open "GET", Uri , True

	objXmlHttp.send

		''wait
		If objXmlHttp.readyState <> READY Then
			objXmlHttp.waitForResponse Timeout
		End If

	   ''not ready yet...so set to error state
		if objXmlHttp.readyState <> READY then
			error = 1
		end if

		If error = 1 Then
			objXmlHttp.Abort
			strHTML = CANT_GET
		Else
			strHTML = Trim(objXmlHttp.responseText)
		End If

	Set objXmlHttp = Nothing
	GetRemoteUriContents = strHTML
end function


'**********************************************************************
'	DESCRIPTION:
'		Returns an arary of short stock status message. e.g. In Stock, Call
'	NOTES:
'		Makes an http webservice call.
'
Function GetShortStockStatusMessages(productIdsArray, companyId)
	dim uri
	const WAIT_TIME = 10
	dim returnXml
	dim nodes
	dim mydoc
	dim ids
	dim returnValues()
	dim I
	dim positionCount
	
	positionCount = 1
	
	'put product array into GET string
	For I = LBound(productIdsArray) To UBound(productIdsArray)
	 if positionCount = 1 Then
 		 ids = ids &  "?" + "productIds=" & productIdsArray(I) 
		 positionCount = 300
	 else
		 ids = ids &  "&" & "productIds=" & productIdsArray(I) 
	 End if
	Next 


	uri = "http://" + GetSiteName(webservice_cdw_com) + "/CDW/BusinessObjects/Product/ProductWebService.asmx/GetStockStatusShortMessages" & ids & "&companyId=" & companyId & "&messageToGet=1"
	returnXml = GetRemoteUriContents(uri, WAIT_TIME)

	

	Set mydoc = Server.CreateObject("Microsoft.XMLDOM")
	set nodes = Server.CreateObject("Microsoft.XMLDOM")

	'load xml into dom
	mydoc.loadXml returnXml

	'get nodes by name string
	set nodes = mydoc.getElementsByTagName("string")

	
	ReDim returnValues(nodes.Length-1)
	
	'parse strings out of xml nodes and place into array
	For I = 0 To nodes.Length-1
		returnValues(I) = nodes.item(I).text
	Next
		
		

	set mydoc = nothing
	set nodes = nothing

	GetShortStockStatusMessages = returnValues
end function


'***************************************************************
'	DESCRIPTION:
'		Returns the Product type
'		Returns ProductGroup/NULL

Function GetProductType(ProductID)
	Dim returnValue
	returnValue = null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ConnectionType_Product)
		Set Cmd = CreateSQLProcedure(ConnectionType_Product, "WebDB.dbo.Product_GetProductType")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)	'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductType", adChar, adParamOutput, 1)		'2
		Call ExecuteSQLProcedure(ConnectionType_Product, Cmd, False)
		If(Cmd.Parameters(0).Value = 0) Then
			returnValue = Cmd.Parameters(2).Value
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ConnectionType_Product)	
	End If
	GetProductType = returnValue
End Function

'**************************************************************
Const prProductProductType				= 0
Const prProductProductClass		    	= 1
Const prProductProductGroup 			= 2
'***************************************************************
'	DESCRIPTION:
'		Returns the Product type, class and Group
'		Returns ProductGroup/NULL

Function GetProductTypeClassGroup(ProductID)
	Dim returnValue, ary(2)
	returnValue = Null
	If(Not(IsEmpty(ProductID)) and Not(IsNull(ProductID)) and IsNumeric(ProductID)) Then
		Dim Cmd
		Call OpenSQLConnection(ctReadWrite)
		Set Cmd = CreateSQLProcedure(ctReadWrite, "WebDB.dbo.Product_GetProductTypeClassGroup")
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductID", adInteger, adParamInput,, ProductID)		'1
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductType", adChar, adParamOutput, 1)			'2
		Cmd.Parameters.Append Cmd.CreateParameter("@ProductClass", adVarChar, adParamOutput, 2)	'3
        Cmd.Parameters.Append Cmd.CreateParameter("@ProductGroup", adVarChar, adParamOutput, 3)	'4
		Call ExecuteSQLProcedure(ctReadWrite, Cmd, False)

		If(Cmd.Parameters(0).Value = 0) Then
			ary(prProductProductType)	= Cmd.Parameters(2).Value
			ary(prProductProductClass)	= Cmd.Parameters(3).Value
			ary(prProductProductGroup)	= Cmd.Parameters(4).Value
			returnValue = ary
		End If
		Set Cmd = Nothing
		Call CloseSQLConnection(ctReadWrite)
	End If
	GetProductTypeClassGroup = returnValue
End Function

%>
