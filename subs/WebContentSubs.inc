<!--#INCLUDE VIRTUAL="/global/subs/FlashSubs.inc"-->
<%
'**************************************************************************************
'Constants for the Weighted Random Include section
	const wfaDirName		= 0
	const wfaFileName		= 1
'Constants for the conditional content section
	const cccEquals			= 0
	const cccNotEquals		= 1
'Constants for the Flash File Properties
	const ffpName			= 0
	const ffpQuality		= 1
	const ffpHeight			= 2
	const ffpWidth			= 3
	const ffpLoop			= 4
'Constants for Individual Function Properties
	const ifpName			= 0
	const ifpValue			= 1
'Constants for the Blue Nugget Header Properties
	const bnpImage			= 0		'Open Nugget
	const bnpTitle			= 1		'Open Nugget/Mac Page Link
	const bnpHeight			= 2		'Open Nugget
'Constants for the Mac Special Link Properties
	const bnpPosition		= 0		'Mac Special Link
	const bnpBoldText		= 1		'Mac Special Link
	const bnpRegularText	= 2		'Mac Special Link
'Constants for the Mac Page Link Properties
	const bnpLinkTitle		= 0		'Mac Page Link
	const bnpLinkURL		= 1		'Mac Page Link
	const bnpIsBold			= 2		'Mac Page Link
'**************************************************************************************
'This function will return an HTML String for a random include.
'	Pass in the desired directory and it will randomly pick on file to display.

Function DrawWebContentRandomIncludeFile(Directory)
	dim fileObj, fileList, docs , allFiles, thisName, filecount, thisNameLen, fileType, thisExt, myfile
	dim FileListString, ReturnValue
	fileCount = 0
	Set fileObj = Server.CreateObject("Scripting.FileSystemObject")
	On Error Resume Next
	Set fileList = fileObj.GetFolder(Server.MapPath("/webcontent" & directory))
	Set allFiles = fileList.Files

		For each docs in allFiles
			thisName = ""
			thisName = thisName & docs.name
			thisNameLen = len(thisName)

			Set fileType = fileObj.GetFile("/webcontent" & directory  & thisName)

			thisExt = Right(ThisName, 4)
			thisName = Left(ThisName, trim(thisNameLen) - 5)

			if UCase(thisExt) = "HTM" OR UCase(thisExt) = "HTML" then
				FileListString	= FileListString & directory & thisName & "." & thisExt & "---"
			End If
		Next

		FileListString = Left(FileListString, Len(FileListString) -3)

		Dim TextFile, RandomSpecial, FileArray

		FileArray = Split(FileListString, "---")

		If IsArray(FileArray) Then
			RandomSpecial = RandomNumber(0,Ubound(FileArray))
			ReturnValue = GetWebcontentIncludeFileStringRecursion(FileArray(RandomSpecial), 1)
		End If

		DrawWebContentRandomIncludeFile = ReturnValue
End Function

'**************************************************************************************
'This function will produce the HTML string that will be displayed on the page.
'	This generic function calls a second function that actually does the work.
'	It passes in a "level" so the file can't include more than 10 levels or recursion.
Function GetWebcontentIncludeFileString(FilePath)
	Dim Level, ReturnValue
		Level = 1

		ReturnValue = GetWebcontentIncludeFileStringRecursion(FilePath, Level)

	GetWebcontentIncludeFileString = returnvalue
end function

'**************************************************************************************
'This function will produce the HTML string that will be displayed on the page.
'	It will loop through the file to remove all special tags
Function GetWebcontentIncludeFileStringRecursion(FilePath, Level)
	Dim myFileObject, myFile, returnValue
	Dim ISStartingPosition, ISEndingPosition, ISLength, StartString, ImageString, EndString
	Dim LenBeforeIS, LenAfterIS, FileLength, FileString

	returnValue = Null
	FilePath = Server.MapPath("/webcontent" & FilePath)

	set myFileObject = Server.Createobject("Scripting.FileSystemObject")

	if myFileObject.FileExists(FilePath) then
	
		'open the file for reading
		set myFile = myFileObject.OpenTextFile(filepath,1)
		if not myFile.AtEndOfStream then
			FileString = myFile.ReadAll()

			returnValue = FileString

			returnValue	= ParseImagePath(returnValue)				' replace the content with the full Image Path
			returnValue	= ParseWebObjectsPath(returnValue)			' replace the content with the full WebObjects Path
			returnValue	= ParseScene7ImagePath(returnValue)			' replace the content with the full WebObjects2 Path
			returnValue	= ParseCompanyName(returnValue)				' replace the content with the current CompanyName

			'ParseFlashFile MUST happen before parsing ParseIncludeFile since it includes an Cdw.IncludeFileTag...
			returnValue	= ParseFlashFile(returnValue)				' replace the code with the IncludeFlashFiles function (below)

			returnValue	= ParseProductImage(returnValue)			' replace the content with the correct product image tag/location
			returnValue	= ParseManufacturerLogo(returnValue)		' replace the content with the correct mfg logo

			returnValue	= ParseIncludeFile(returnValue, Level)		' include up to 10 include files 
			returnValue	= ParseConditionalWebContent(returnValue)	' display only the code that applies to this site.

			If glMacWarehouseUser Then
				returnValue	= ParseMacWarehouseNugget(returnValue)		' display the macwarehouse nuggets.
			End if

			'**************************************************************************************************
			'***	The following section will replace commonly uses URL tags with the appropriate URL.		***
			'***	The URL list is maintained in /global/subs/global.inc AND in CDWMain.					***
			'**************************************************************************************************
			'Eaccount Pages
				returnValue = Replace(returnValue, "<Cdw.LoginURL>", glLoginPageURL)
				returnValue = Replace(returnValue, "<Cdw.AccountDetailsURL>", glAccountDetailsURL)
				returnValue = Replace(returnValue, "<Cdw.AccountTeamURL>", glAccountTeamURL)
				returnValue = Replace(returnValue, "<Cdw.CreateAccountURL>", glCreateAccountURL)
			'Home Page/Extranet Home Page
				returnValue = Replace(returnValue, "<Cdw.HomePageURL>", glHomepageURL)
				returnValue = Replace(returnValue, "<Cdw.ExtranetHomePageURL>", glMyOrganizationURL)
			'Order Status/My Purchases pages...
				returnValue = Replace(returnValue, "<Cdw.OrderStatusUrl>", glOrderStatusURL)
				returnValue = Replace(returnValue, "<Cdw.OrderDetailsUrl>", glOrderDetailsURL)
				returnValue = Replace(returnValue, "<Cdw.InvoiceListUrl>", glInvoiceListURL)
				returnValue = Replace(returnValue, "<Cdw.InvoiceDetailsUrl>", glInvoiceDetailsURL)
				returnValue = Replace(returnValue, "<Cdw.MyPurchasesUrl>", glMyPurchasesURL)
				returnValue = Replace(returnValue, "<Cdw.PurchaseHistoryUrl>", glPurchaseHistoryURL)
				returnValue = Replace(returnValue, "<Cdw.FinanceCenterUrl>", glFinanceCenterURL)
			'Product related Pages
				returnValue	= Replace(returnValue, "<Cdw.ProductPageUrl>", glProductURL)
				returnValue	= Replace(returnValue, "<Cdw.ProductAccessoriesPageUrl>", glAccessoriesURL)
				returnValue	= Replace(returnValue, "<Cdw.ProductRebatesPageUrl>", glRebatesURL)
				returnValue	= Replace(returnValue, "<Cdw.ProductRecommendationsPageUrl>", glRecommendationsURL)
				returnValue	= Replace(returnValue, "<Cdw.ProductReviewPageURL>", glProductReviewURL)
			'Product Finders
				returnValue = Replace(returnValue, "<Cdw.ProductFinderURL>", glProductFinderURL)
				returnValue = Replace(returnValue, "<Cdw.ProductFinderResultsURL>", glProductFinderResultsURL)
				returnValue = Replace(returnValue, "<Cdw.InkAndTonerFinderURL>", glInkAndTonerURL)
			'Quotes related Pages
				returnValue	= Replace(returnValue, "<Cdw.QuotesListUrl>", glQuotesListURL)
				returnValue	= Replace(returnValue, "<Cdw.QuoteDetailsUrl>", glQuoteDetailsURL)
			'Rebates pages
				returnValue	= Replace(returnValue, "<Cdw.RebateDefaultPageUrl>", glRebateDefaultURL)
				returnValue	= Replace(returnValue, "<Cdw.RebateDetailPageUrl>", glRebateDetailsURL)
			'Search related Pages
				returnValue	= Replace(returnValue, "<Cdw.SmartSearchPageUrl>", glSmartSearchURL)
				returnValue	= Replace(returnValue, "<Cdw.SearchPageUrl>", glSearchResultsURL)
			'Shop By Brand pages
				returnValue	= Replace(returnValue, "<Cdw.SBBVendorPageUrl>", glSBBVendorURL)
				returnValue = Replace(returnValue, "<Cdw.SBBDefaultPageURL>", glSBBDefaultURL)
			'Shopping Cart/Checkout related Pages
				returnValue	= Replace(returnValue, "<Cdw.ShoppingCartPageUrl>", glShoppingCartURL)
				returnValue	= Replace(returnValue, "<Cdw.AddCartItemUrl>", glAddCartItemURL)
				returnValue	= Replace(returnValue, "<Cdw.AddContractItemUrl>", glAddContractItemURL)
				returnValue	= Replace(returnValue, "<Cdw.ConvertShoppingCartPageUrl>", glConvertCartURL)
				returnValue	= Replace(returnValue, "<Cdw.CheckoutReviewPageUrl>", glReviewURL)
			'Remaining misc. pages
				returnValue	= Replace(returnValue, "<Cdw.ComparePageUrl>", glComparePageURL)
				returnValue	= Replace(returnValue, "<Cdw.HubPageUrl>", glHubPageURL)
				returnValue = Replace(returnValue, "<Cdw.ContractExplorerURL>", glContractExplorerURL)
				returnValue = Replace(returnValue, "<Cdw.MarketingBundleURL>", glMarketingBundleURL)
		end if
		set myFile = Nothing
	end if
		set myFileObject = Nothing

	GetWebcontentIncludeFileStringRecursion = returnvalue
end function

'**************************************************************************************
Function DrawWebContentRandomIncludeFileWithWeight(Directory)
	Dim StartPosition, EndPosition, wfa, RandomSpecial, ReturnValue

	StartPosition	= null
	EndPosition		= null

	If IsArray(WeightedFileArray) Then
		For wfa = 0 to Ubound(WeightedFileArray)
			If Trim(LCase(WeightedFileArray(wfa,wfaDirName))) = LCase(Trim(Directory)) Then
				If Not(IsNumeric(StartPosition)) Then
					StartPosition = wfa
				End If
				EndPosition = wfa
			End If
		Next

		If IsNumeric(StartPosition) And IsNumeric(EndPosition) Then
			RandomSpecial	= RandomNumber(StartPosition,EndPosition)
			ReturnValue		= GetWebcontentIncludeFileStringRecursion(WeightedFileArray(RandomSpecial,1), 1)
		Else
			ReturnValue		= DrawWebContentRandomIncludeFile(Directory)
		End If
	Else
		ReturnValue = DrawWebContentRandomIncludeFile(Directory)
	End If

	DrawWebContentRandomIncludeFileWithWeight = ReturnValue
End Function

'**************************************************************************************
' This function gets the configuration file from the directory of the random include...
Function ReadConfigurationFile(directory)
	dim physicalPath, myFileObject, myFile, returnValue
	returnValue = Null

	physicalPath = server.MapPath("/webcontent" & directory & "Config.txt")
	set myFileObject = Createobject("Scripting.FileSystemObject")

	if myFileObject.FileExists(physicalPath) then
		set myFile = myFileObject.OpenTextFile(physicalPath)

		if not myFile.AtEndOfStream then
			returnValue = myFile.ReadAll()
		end if
		set myFile = Nothing
	end if
	set myFile = Nothing
	set myFileObject = Nothing

	If Not(IsNull(returnValue)) AND Not(IsEmpty(returnValue)) AND Len(returnValue) > 0 Then
	'Replace line breaks, spaces and ;; for a cleaner version of the file
		returnValue = Replace(returnValue, " ", "")
		returnValue = Replace(returnValue, chr(10), ";")
		returnValue = Replace(returnValue, chr(13), ";")
		returnValue = Replace(returnValue, ";;", ";")
	End If

	ReadConfigurationFile = returnvalue
end function

'***********************************************************************************************************
'	Functions to strip out special tags
'	Replace the special tags with the new, improved tags so the pages can be more dynamic.

'	Strip out all the Cdw.ImagePath Tags
Function ParseImagePath(html)
	Dim returnValue, tag, imagePath
		returnValue	= html
		tag			= "Cdw.ImagePath"

		'Replace all the <Cdw.ImagePath> tags with the proper image path...
		Do While (DoesPairedTagExist( returnValue, tag ))
			imagePath	= GetTagFromHtml( returnValue, tag, 1 )
			returnValue	= ReplaceTagInHtml( returnValue, tag, GetImagePath(imagePath), 1 )
		Loop

	ParseImagePath	= returnValue
End Function
'***********************************************************************************************************
'  Strip out all the Cdw.WebObjectsPath Tags
Function ParseWebObjectsPath(html)
	Dim returnValue, tag, WebObjectsPath
		returnValue	= html
		tag			= "Cdw.WebObjectsPath"

		'Replace all the <Cdw.WebObjectsPath> tags with the proper webobjects path...
		Do While (DoesPairedTagExist( returnValue, tag ))
			WebObjectsPath	= GetTagFromHtml( returnValue, tag, 1 )
			returnValue		= ReplaceTagInHtml( returnValue, tag, GetWebObjectPath(WebObjectsPath), 1 )
		Loop

	ParseWebObjectsPath	= returnValue
End Function

'***********************************************************************************************************
'	Functions to strip out special tags
'	Replace the special tags with the new, improved tags so the pages can be more dynamic.

'	Strip out all the Cdw.Scene7ImagePath Tags
Function ParseScene7ImagePath(html)
	Dim returnValue, tag, Scene7ImagePath
		returnValue	= html
		tag			= "Cdw.Scene7ImagePath"

		'Replace all the <Cdw.Scene7ImagePath> tags with the proper image path...
		Do While (DoesPairedTagExist( returnValue, tag ))
			Scene7ImagePath	= GetTagFromHtml( returnValue, tag, 1 )
			returnValue	= ReplaceTagInHtml( returnValue, tag, GetScene7Path(Scene7ImagePath), 1 )
		Loop

	ParseScene7ImagePath	= returnValue
End Function

'***********************************************************************************************************
'	Strip out all the Cdw.IncludeFile Tags
'		Level indicates a level of recursion.  No more than 10 levels

Function ParseIncludeFile(html, level)
	Dim returnValue, tag, IncludeFile
		returnValue	= html
		tag			= "Cdw.IncludeFile"

		'Replace all the <Cdw.IncludeFile> tags with the proper parsed string path...
		Do While (DoesPairedTagExist( returnValue, tag ))
			If (Level =< 10) Then
				IncludeFile		= GetTagFromHtml( returnValue, tag, 1 )
				returnValue		= ReplaceTagInHtml( returnValue, tag, GetWebcontentIncludeFileStringRecursion(IncludeFile, Level + 1), 1 )
			End If
		Loop

	ParseIncludeFile	= returnValue
End Function

'***********************************************************************************************************
'  Strip out all the Cdw.CompanyName Tags
Function ParseCompanyName(html)
	Dim returnValue, tag, imagePath
		returnValue	= html
		tag			= "Cdw.CompanyName"

		'Replace all the <Cdw.CompanyName> tags with the proper Company Name...
		Do While ( DoesPairedTagExist( returnValue, tag) )
			if GetThisSite() = www_cdwg_com Then
				returnValue = ReplaceTagInHtml( returnValue, tag, "CDW&middot;G", 1 )
			elseif GetThisSite() = www_cdw_ca Then
				returnValue = ReplaceTagInHtml( returnValue, tag, "CDW Canada", 1 )
			else
				returnValue = ReplaceTagInHtml( returnValue, tag, "CDW", 1 )
			end If
		Loop

	ParseCompanyName	= returnValue
End Function

'**************************************************************************************
' This function will replace all instances of Cdw.FlashFile with the appropriate code.
' Changed on 11/08/2005 by Paul Haan
'	Removed some function calls from the previous version of the ParseFlashFile functions
'	Replaced the function calls with an XML reader to get the data straight out of the XML

function ParseFlashFile(html)
	Dim returnValue, FlashFileXML, flashContent, tag, FlashFileVariablesArray
	Dim NoFlashFile, FlashFile, FlashQuality, FlashHeight, FlashWidth, FileLoop
	Dim XMLError
		XMLError = false
		returnValue	= html
		tag			= "Cdw.FlashFile"
		NoFlashFile	= ""
		
	'Replace all the <Cdw.FlashFile> tags with the proper Company Name...
	Do While (DoesPairedTagExist( returnValue, tag) )

		FlashFileXML	= GetHtmlTagOuter( returnValue, tag, 1 )
		flashContent	= ""

			dim xmlDoc, attribute

			Set XmlDoc = Server.CreateObject("MSXML2.DOMDocument")
			xmlDoc.loadXML(FlashFileXML)
			if (xmlDoc.parseError.errorCode > 0) Then
				XMLError = true
			End If

			if XMLError = false Then
				for each attribute in xmlDoc.documentElement.firstChild.attributes
					if lcase(attribute.NodeName) = "filename" Then FlashFile = attribute.text
					if lcase(attribute.NodeName) = "quality" Then FlashQuality = attribute.text
					if lcase(attribute.NodeName) = "height" Then FlashHeight = attribute.text
					if lcase(attribute.NodeName) = "width" Then FlashWidth = attribute.text
					if lcase(attribute.NodeName) = "loop" Then FileLoop = attribute.text
				next

				NoFlashFile = xmlDoc.documentElement.firstChild.nextSibling.firstChild.text
				If Instr(lcase(NoFlashFile), "/webcontent") = 0 Then
					NoFlashFile = "/webcontent" & Trim(NoFlashFile)
				End If
			Else
				flashContent = ""
			End If

			Set XmlDoc = nothing
		flashContent = IncludeFlashFiles(NoFlashFile, FlashFile, FlashQuality, FlashHeight, FlashWidth, FileLoop)

		'This will remove the <Cdw.FlashFile></Cdw.FlashFile> tags from the file string.
		returnValue = ReplaceTagInHtml( returnValue, tag, flashContent, 0 )
	Loop

	ParseFlashFile = returnValue
end function

'**************************************************************************************
' This function will check to see if the code should be displayed on the current site
function ParseConditionalWebContent(html)
	dim returnValue, tag, conditionalWebContentTag, condition, content
		returnValue = html

	' Check the Conditions on the <Cdw.ConditionalWebContent> tag (if there is one) and remove if necessary...
	tag = "Cdw.ConditionalWebContent"
	Do While ( DoesPairedTagExist( returnValue, tag ) )
		conditionalWebContentTag = GetTagFromHtml( returnValue, tag, 1 )
		condition = GetTagFromHtml( conditionalWebContentTag, "Condition", 1 )
		content = GetTagFromHtml( conditionalWebContentTag, "WebContent", 1 )

		if IsConditionTrue(condition) = True Then
			returnValue = ReplaceTagInHtml( returnValue, tag, content, 1 )
		else
			returnValue = ReplaceTagInHtml( returnValue, tag, "", 1 )
		End If
	Loop

	ParseConditionalWebContent = returnValue
end function

'**************************************************************************************
' This function will check to see if the code should be displayed on the current site
function IsConditionTrue(conditionString)
	dim returnValue, ConditionArray, leftside, rightside, operand, ConditionCheck, codeArray, ca
		returnValue = false

	ConditionArray = Split(Trim(conditionString), " ")
	if Ubound(ConditionArray) <> 2 Then
		returnValue = false
	else
		leftside	= ConditionArray(0)
		operand		= ConditionArray(1)
		rightside	= ConditionArray(2)

		ConditionCheck = cccEquals
		if ( operand = "!=" ) Then
			ConditionCheck = cccNotEquals
		End If

		' UICode test
		if ( Ucase(leftside) = "UICODE" ) Then
			codeArray = Split(UCase(rightside), ",")

			if ( ConditionCheck = cccEquals ) Then
				returnValue = false

				For ca = 0 to Ubound(CodeArray)
					if ( CodeArray(ca) = "C" AND GetThisSite() = www_cdw_com ) Then
						returnValue = true
					elseif ( CodeArray(ca) = "G" AND GetThisSite() = www_cdwg_com ) Then
						returnValue = true
					elseif ( CodeArray(ca) = "CA" AND GetThisSite() = www_cdw_ca) Then
						returnValue = true
					elseif ( CodeArray(ca) = "MW" AND glMacwarehouseUser = True )Then
						returnValue = true
					end if
				Next
			End If

			if ( ConditionCheck = cccNotEquals ) Then
				returnValue = true

				For ca = 0 to Ubound(CodeArray)
					if ( CodeArray(ca) = "C" AND GetThisSite() = www_cdw_com ) Then
						returnValue = false
					elseif ( CodeArray(ca) = "G" AND GetThisSite() = www_cdwg_com )Then
						returnValue = false
					elseif ( CodeArray(ca) = "CA" AND GetThisSite() = www_cdw_ca )Then
						returnValue = false
					elseif ( CodeArray(ca) = "MW" AND glMacwarehouseUser = True )Then
						returnValue = false
					end if
				Next
			End If
		End If
	End If

	IsConditionTrue	= returnValue
End Function

'**************************************************************************************
'These tags are used to search and/or replace the HTML for existing tags
'**************************************************************************************
Function ReplaceTagInHtml(html, tag, textToInsert, startPos)
	'Response.write("<hr />ReplaceTagInHtml(<hr /><!--" & HTMLEncode(html) & "<hr />-->" & HTMLEncode(tag) & "<hr />" & HTMLEncode(textToInsert) & "<hr />" & startPos & ")<hr />")
	dim beginTag, endTag, returnValue
	dim pos1, pos2

		beginTag = "<" & UCase(tag) & ">"
		endTag = "</" & UCase(tag) & ">"
		returnValue = ""

		pos1 = InStr( UCase(html), beginTag )
		if (pos1 >= 0) Then
			returnValue = Mid(html, 1, (pos1 - 1))	' Subtract one or the "<" from beginTag will not be removed

			pos1 = pos1 + Len(beginTag)
			pos2 = InStr(pos1, UCase(html), endTag )
			if ( pos2 >= 0 ) then
				pos2 = pos2 + Len(endTag)
				returnValue = returnValue & textToInsert & Mid(html, pos2, (Len(html) - pos2) + 1 )	'Add one back since we had to subtract it above
			End If
		Else
			returnValue = html
		End If
	ReplaceTagInHtml = returnValue
End Function

'**************************************************************************************
Function GetTagFromHtml(html, tag, startPos)
'	Response.write("GetTagFromHtml(<br />" & HtmlEncode(html) & "<hr />" & HTMLEncode(tag) & "<hr />" & startPos & ")<hr />")

	dim beginTag, endTag, returnValue
	dim pos1, pos2, StringLength
		beginTag = "<" & Ucase(tag) & ">"
		endTag = "</" & Ucase(tag) & ">"

		pos1	= InStr( UCase(html), beginTag )
		if ( pos1 >= 0 ) Then
			pos1 = pos1 + Len(beginTag)

			pos2 = InStr( pos1, UCase(html), endTag )
			StringLength = Pos2 - Pos1

			if ( pos2 >= 0 ) Then
				returnValue = Mid(html, ( pos1 ), StringLength )
			End If
		Else
			returnValue = ""
		End If
	GetTagFromHtml = returnValue
End Function

'**************************************************************************************
function GetHtmlTagOuter(html, tag, startPos)
	Dim returnValue, beginTag, endTag, pos1, pos2, StringLength
		ReturnValue = ""
		beginTag = "<" & UCase(tag) & ">"
		endTag = "</" & UCase(tag) & ">"

	pos1	= InStr( UCase(html), beginTag )
	if ( pos1 >= 0 ) Then
		pos2 = InStr(pos1 , UCase(html), endTag )
		StringLength = Pos2 - Pos1

		if ( pos2 >= 0 ) Then
			returnValue = Mid(html, ( pos1 ), StringLength + Len(EndTag) )
		End If
	End If

	GetHtmlTagOuter = returnValue
end function

'**************************************************************************************
'This function will check for the a specific single tag in the HTML
'	If they do not exist, then no more processing is needed
function DoesSingleTagExist(html, tag)
	Dim beginTag, ReturnValue
		beginTag	= "<" & lcase(tag) & ">"
		ReturnValue = false

		If InStr(lcase(html), beginTag) > 0 Then ReturnValue = true End If
	DoesSingleTagExist = ReturnValue
End function

'**************************************************************************************
'This function will check for the a specific paired tag in the HTML
'	If they do not exist, then no more processing is needed
function DoesPairedTagExist(html, tag)
	Dim beginTag, endTag, ReturnValue

		beginTag	= "<" & UCASE(tag) & ">"
		endTag		= "</" & UCASE(tag) & ">"
		ReturnValue = false

		If InStr(UCASE(html), beginTag) > 0 And InStr(UCASE(html), endTag) > 0 Then ReturnValue = true End If

	DoesPairedTagExist = ReturnValue
End function

'***********************************************************************************************************
'  Strip all tabs and returns
Function StripSpacing(TextString)
	Dim ReturnValue
		ReturnValue = TextString

		ReturnValue = Replace(ReturnValue, chr(10), "")
		ReturnValue = Replace(ReturnValue, chr(13), "")
		ReturnValue = Replace(ReturnValue, chr(9), "")

	StripSpacing  = ReturnValue
End Function

'***********************************************************************************************************
'*	The functions below all deal with the right sidebar nuggets for the MacWarehouse site.				   *
'***********************************************************************************************************
'  Get the MacWarehouse Nugget Header.
'	This is for the Mac Warehouse Nuggets.  
'	Returns an array of properties for the function.
Function ParseMacWarehouseNuggetHeader(SectionCode, FullCode)
	Dim NuggetHeaderPropertyArray(2), ReturnValue, PropertyArray, j, IndividualProperty
	Dim NuggetHeader, NuggetGraphic, NuggetTitle, NuggetHeight
	Dim ParseSection, tag

		' Strip all the ", ', tabs and line returns
		ReturnValue		= FullCode
		ParseSection	= SectionCode
		ParseSection	= Replace(ParseSection, """", "")
		ParseSection	= Replace(ParseSection, "'", "")
		ParseSection	= StripSpacing(ParseSection)
		tag				= "Nugget.Header"

	Do While (DoesPairedTagExist( ParseSection, tag) )
		PropertyArray = Split(ParseSection, " ")

		If IsArray(PropertyArray) Then
			for j=0 to Ubound(PropertyArray)
				NuggetGraphic	= ""
				NuggetTitle		= ""
				NuggetHeight	= ""

				if instr(PropertyArray(j), "=") Then
					IndividualProperty = Split(PropertyArray(j), "=")
					If IsArray(IndividualProperty) Then
						If Lcase(IndividualProperty(ifpName)) = "image" Then NuggetHeaderPropertyArray(bnpImage)	= IndividualProperty(ifpValue) End If
						If Lcase(IndividualProperty(ifpName)) = "title" Then NuggetHeaderPropertyArray(bnpTitle)	= IndividualProperty(ifpValue) End If
						If Lcase(IndividualProperty(ifpName)) = "height" Then NuggetHeaderPropertyArray(bnpHeight)	= IndividualProperty(ifpValue) End If
					End If
				End If
			Next

			If IsArray(NuggetHeaderPropertyArray) Then
				NuggetGraphic		= NuggetHeaderPropertyArray(bnpImage)
				NuggetTitle			= NuggetHeaderPropertyArray(bnpTitle)
				NuggetHeight		= NuggetHeaderPropertyArray(bnpHeight)
				NuggetHeader		= RenderBlueNuggetOpen(NuggetGraphic, NuggetTitle, NuggetHeight)

				ParseSection		= ReplaceTagInHtml( ParseSection, tag, NuggetHeader, 0 )
				returnValue			= ReplaceTagInHtml( returnValue, tag, NuggetHeader, 0 )
			End If
		End If
	Loop

	ParseMacWarehouseNuggetHeader	= ReturnValue
End Function

'***********************************************************************************************************
'  Get the MacWarehouse Nugget Initial Special for the Rollover section.
'	This is for the Mac Warehouse Nuggets.  
'	Returns an EDC to be passed into a function.
Function ParseMacWarehouseInitialSpecial(html)
	Dim ReturnValue, PropertyArray, j, IndividualProperty, EDC

		' Strip all the ", ', tabs and line returns
		html = Replace(HTML, """", "")
		html = Replace(HTML, "'", "")
		html = StripSpacing(html)

		html = Replace(html, "<Nugget.Content.SubTable.Specials.InitialSpecial>", "")
		html = Replace(html, "</Nugget.Content.SubTable.Specials.InitialSpecial>", "")
		PropertyArray = Split(HTML, " ")

		If IsArray(PropertyArray) Then
			for j=0 to Ubound(PropertyArray)
				if instr(PropertyArray(j), "=") Then
					IndividualProperty = Split(PropertyArray(j), "=")
					If IsArray(IndividualProperty) Then
						If Lcase(IndividualProperty(ifpName)) = "edc" Then ReturnValue	= IndividualProperty(ifpValue) End If
					End If
				End If
			Next
		End If
	ParseMacWarehouseInitialSpecial	= ReturnValue
End Function

'**************************************************************************************
' This function will replace all instances of Cdw.MacWarehouseNugget with the appropriate code.
function ParseMacWarehouseNugget(html)
	Dim returnValue, NuggetXML, tag, FlashFileVariablesArray
	Dim HeaderXML, InitialSpecialXML, SpecialsXML, RegularXML
	Dim NuggetContentXML, NuggetContent
	Dim NuggetFooterXML, NuggetFooter
	Dim MacWarehouseNugget, InitialEDC, InitialSpecial
	Dim Graphic, Title, NuggetHeight, MacSpecialLinks
	Dim FullNugget

		returnValue	= html
		tag			= "Cdw.MacWarehouseNugget"

	'Replace all the <Cdw.MacWarehouseNugget> tags with the proper text...
	Do While (DoesPairedTagExist( returnValue, tag) )
'		Response.write(HTMLEncode(ReturnValue) & "<hr />")

		NuggetXML				= GetHtmlTagOuter( returnValue, tag, 1 )

		'Do the work for the Header of the nugget...
		Do While (DoesPairedTagExist( returnValue, "Nugget.Header") )
			HeaderXML			= GetHtmlTagOuter( returnValue, "Nugget.Header", 1 )
			ReturnValue			= ParseMacWarehouseNuggetHeader(HeaderXML, ReturnValue)
		Loop

		Do While (DoesPairedTagExist( returnValue, "Nugget.Content") )
			If DoesPairedTagExist( ReturnValue, "Nugget.Content.SubTable.Specials.InitialSpecial") = True Then
				Do While (DoesPairedTagExist( returnValue, "Nugget.Content.SubTable.Specials.InitialSpecial") )
					InitialSpecialXML	= GetHtmlTagOuter( returnValue, "Nugget.Content.SubTable.Specials.InitialSpecial", 1 )
					InitialEDC			= ParseMacWarehouseInitialSpecial(InitialSpecialXML)
					InitialSpecial		= DrawInitialMacSpecial(InitialEDC)
					returnValue			= ReplaceTagInHtml( returnValue, "Nugget.Content.SubTable.Specials.InitialSpecial", InitialSpecial, 0 )
				Loop
			End If

			'Process the Content of the nugget...
				Do While(DoesSingleTagExist(ReturnValue, "Nugget.Content.SubTable.Regular.Open"))
					ReturnValue = Replace( ReturnValue, "<Nugget.Content.SubTable.Regular.Open>", RegularNuggetSpacingTable)
				Loop
				Do While(DoesSingleTagExist(ReturnValue, "Nugget.Content.SubTable.Specials.Open"))
					ReturnValue = Replace( ReturnValue, "<Nugget.Content.SubTable.Specials.Open>", "<table border='0' cellspacing='0' cellpadding='0' height='100%'>")
				Loop
				Do While(DoesSingleTagExist(ReturnValue, "Nugget.Content.SubTable.End"))
					ReturnValue = Replace( ReturnValue, "<Nugget.Content.SubTable.End>", "</table>")
				Loop

				If DoesPairedTagExist( ReturnValue, "Nugget.Content.RolloverLinks") = True Then
					Do While(DoesPairedTagExist(ReturnValue, "Nugget.Content.RolloverLinks"))
						SpecialsXML			= GetHtmlTagOuter( returnValue, "Nugget.Content.RolloverLinks", 1 )
						ReturnValue			= ParseMacSpecialLink(SpecialsXML, ReturnValue)
					Loop
				End If
				If DoesPairedTagExist( ReturnValue, "Nugget.Content.RegularLinks") = True Then
					Do While(DoesPairedTagExist(ReturnValue, "Nugget.Content.RegularLinks"))
						RegularXML			= GetHtmlTagOuter( returnValue, "Nugget.Content.RegularLinks", 1 )
						ReturnValue			= ParseMacRegularLink(RegularXML, ReturnValue)
					Loop
				End If

				NuggetContent	= GetTagFromHtml( returnValue, "Nugget.Content", 1 )
				returnValue		= ReplaceTagInHtml( returnValue, "Nugget.Content", NuggetContent, 0 )
			'End Nugget Content Process
		Loop

			'Process the footer of the nugget...
				Do While (DoesPairedTagExist( returnValue, "Nugget.Footer") )
					If Instr(Lcase(html), "<closebluenugget>") Then
						returnValue = ReplaceTagInHtml( returnValue, "Nugget.Footer", RenderBlueNuggetClose, 0 )
					End If
				Loop

		'MacWarehouseNugget = NuggetHeader & NuggetContent & NuggetFooter

		'This will remove the <Cdw.MacWarehouseNugget></Cdw.MacWarehouseNugget> tags from the file string.

			FullNugget	= GetTagFromHtml( returnValue, tag, 1 )
			returnValue = ReplaceTagInHtml( returnValue, tag, FullNugget, 0 )
	Loop

	ParseMacWarehouseNugget = returnValue
end function

'**************************************************************************************
function ParseMacSpecialLink(SectionCode, FullCode)
	Dim returnValue, MacSpecialLinkXML, tag, ParseSection
	Dim MacSpecialLinks, SpecialLinkArray, j, k
	Dim LinkPosition, LinkBText, LinkRText, CurrentMacSpecialLink
	Dim IndividualProperty, SpecialLinkPropertyArray(2), SpecialLinkProperties

		returnValue		= FullCode
		ParseSection	= SectionCode
		tag				= "Nugget.Content.RolloverLinks"
		MacSpecialLinks	= ""

		ParseSection	= StripSpacing(ParseSection)
		ParseSection	= Replace(ParseSection, """", "")

	'Replace all the <Nugget.Content.RolloverLinks> tags with the proper text...
	Do While (DoesPairedTagExist( ParseSection, tag) )
		MacSpecialLinkXML	= GetHtmlTagOuter( ParseSection, tag, 1 )

		'Strip out a couple unnecessary tags...
		MacSpecialLinkXML	= Replace(MacSpecialLinkXML, "MacSpecialLink", "")
		MacSpecialLinkXML	= Replace(MacSpecialLinkXML, " /", " ")
		SpecialLinkArray	= Split(MacSpecialLinkXML, "><")

		If IsArray(SpecialLinkArray) Then
			for j=0 to Ubound(SpecialLinkArray)
				if instr(SpecialLinkArray(j), "@") Then
					SpecialLinkProperties = Split(SpecialLinkArray(j), "@")
					If IsArray(SpecialLinkProperties) Then
						for k=0 to Ubound(SpecialLinkProperties)
							if instr(SpecialLinkProperties(k), "=") Then
								IndividualProperty = Split(SpecialLinkProperties(k), "=")
								If IsArray(IndividualProperty) Then
									If Lcase(IndividualProperty(ifpName)) = "pos" Then SpecialLinkPropertyArray(bnpPosition)			= IndividualProperty(ifpValue) End If
									If Lcase(IndividualProperty(ifpName)) = "boldtext" Then SpecialLinkPropertyArray(bnpBoldText)		= IndividualProperty(ifpValue) End If
									If Lcase(IndividualProperty(ifpName)) = "regulartext" Then SpecialLinkPropertyArray(bnpRegularText)	= IndividualProperty(ifpValue) End If
								End If
							End If
						Next
							CurrentMacSpecialLink = ""

							If IsArray(SpecialLinkPropertyArray) Then
								LinkPosition			= Trim(SpecialLinkPropertyArray(bnpPosition))
								LinkBText				= Trim(SpecialLinkPropertyArray(bnpBoldText))
								LinkRText				= Trim(SpecialLinkPropertyArray(bnpRegularText))

								CurrentMacSpecialLink	= DrawMacSpecialLink(LinkPosition, LinkBText, LinkRText)
							End If
					End If

					MacSpecialLinks				= MacSpecialLinks & CurrentMacSpecialLink
				End If
			Next

			If Not(IsNull(MacSpecialLinks)) And Not(IsEmpty(MacSpecialLinks)) AND Len(MacSpecialLinks) > 0 Then
				ParseSection = ReplaceTagInHtml( ParseSection, tag, MacSpecialLinks, 0 )
				ReturnValue = ReplaceTagInHtml( ReturnValue, tag, MacSpecialLinks, 0 )
			End If
		End If
	Loop

	ParseMacSpecialLink = returnValue
end function

'**************************************************************************************
function ParseMacRegularLink(SectionCode, FullCode)
	Dim returnValue, MacRegularLinkXML, tag, ParseSection
	Dim MacRegularLinks, RegularLinkArray, j, k, IsBold
	Dim LinkURL, LinkTitle, CurrentMacRegularLink
	Dim IndividualProperty, RegularLinkPropertyArray(2), RegularLinkProperties

		returnValue		= FullCode
		ParseSection	= SectionCode
		tag				= "Nugget.Content.RegularLinks"

		ParseSection	= StripSpacing(ParseSection)
		ParseSection	= Replace(ParseSection, """", "")

	'Replace all the <Nugget.Content.RegularLinks> tags with the proper text...
	Do While (DoesPairedTagExist( ParseSection, tag) )
		MacRegularLinkXML	= GetHtmlTagOuter( ParseSection, tag, 1 )

		'Strip out a couple unnecessary tags...
		MacRegularLinkXML	= Replace(MacRegularLinkXML, "MacRegularLink", "")
		MacRegularLinkXML	= Replace(MacRegularLinkXML, " /", " ")
		RegularLinkArray	= Split(MacRegularLinkXML, "><")

		If IsArray(RegularLinkArray) Then
			'DumpArray(RegularLinkArray)
			for j=0 to Ubound(RegularLinkArray)
				if instr(RegularLinkArray(j), "@") Then
					RegularLinkProperties = Split(RegularLinkArray(j), "@")
					If IsArray(RegularLinkProperties) Then
						'DumpArray(RegularLinkProperties)
						for k=0 to Ubound(RegularLinkProperties)
							if instr(RegularLinkProperties(k), "=") Then
								IndividualProperty = Split(RegularLinkProperties(k), "=")
								If IsArray(IndividualProperty) Then
									'DumpArray(IndividualProperty)
									If Lcase(IndividualProperty(ifpName)) = "title" Then RegularLinkPropertyArray(bnpLinkTitle)		= IndividualProperty(ifpValue) End If
									If Lcase(IndividualProperty(ifpName)) = "redirect" Then RegularLinkPropertyArray(bnpLinkURL)	= IndividualProperty(ifpValue) End If
									If Lcase(IndividualProperty(ifpName)) = "isbold" Then RegularLinkPropertyArray(bnpIsBold)		= IndividualProperty(ifpValue) End If
								End If
							End If
						Next
					End If

					CurrentMacRegularLink = ""

					If IsArray(RegularLinkPropertyArray) Then
						LinkTitle				= RegularLinkPropertyArray(bnpLinkTitle)
						LinkURL					= RegularLinkPropertyArray(bnpLinkURL)
						IsBold					= RegularLinkPropertyArray(bnpIsBold)
						CurrentMacRegularLink	= DrawPageLink(LinkTitle, LinkURL, IsBold)
					End If

					MacRegularLinks				= MacRegularLinks & CurrentMacRegularLink
				End If
			Next

			If Not(IsNull(MacRegularLinks)) And Not(IsEmpty(MacRegularLinks)) AND Len(MacRegularLinks) > 0 Then
				ParseSection = ReplaceTagInHtml( ParseSection, tag, MacRegularLinks, 0 )
				ReturnValue = ReplaceTagInHtml( ReturnValue, tag, MacRegularLinks, 0 )
			End If
		End If
	Loop

	ParseMacRegularLink = returnValue
end function
'**************************************************************************************

'**************************************************************************************
' This function will replace all instances of Cdw.FlashFile with the appropriate code.
function ParseProductImage(html)
	Dim returnValue, ProductImageXML, ProductImage, tag, ProductImageProperties
	Dim EDC, ImageType, Size, XMLError

		XMLError = false
		returnValue	= html
		tag			= "Cdw.ProductImage"
		ProductImage	= ""
	'Replace all the <Cdw.ProductImage> tags with the proper image tag/location
	Do While (DoesPairedTagExist( returnValue, tag) )

		ProductImageXML	= GetHtmlTagOuter( returnValue, tag, 1 )
		ProductImage	= ""

			dim xmlDoc, attribute

			Set XmlDoc = Server.CreateObject("MSXML2.DOMDocument")
			xmlDoc.loadXML(ProductImageXML)
			if (xmlDoc.parseError.errorCode > 0) Then
				XMLError = true
			End If

			if XMLError = false Then
				for each attribute in xmlDoc.documentElement.firstChild.attributes
					if lcase(attribute.NodeName) = "edc" Then EDC = attribute.text 
					if lcase(attribute.NodeName) = "type" Then ImageType = attribute.text 
					if lcase(attribute.NodeName) = "size" Then Size = attribute.text 
				next

				if (UCase(ImageType) = "TAG") Then
					ProductImage = GetProductImageTagbyCode(EDC, Size)
				else
					ProductImage = GetProductImageLocationbyCode(EDC, Size)
				end if
			Else
				ProductImage = ""
			End If

			Set XmlDoc = nothing

		'This will remove the <Cdw.ProductImage></Cdw.ProductImage> tags from the file string.
		returnValue = ReplaceTagInHtml( returnValue, tag, ProductImage, 0 )
	Loop

	ParseProductImage = returnValue
end function

'**************************************************************************************
' This function will replace all instances of Cdw.FlashFile with the appropriate code.
function ParseManufacturerLogo(html)
	Dim returnValue, ManufacturerLogoXML, ManufacturerLogo, tag, ManufacturerLogoProperties
	Dim MFGCode, MFGName, Size, SiteCode, XMLError

		XMLError = false
		returnValue	= html
		tag			= "Cdw.ManufacturerLogo"
		ManufacturerLogo	= ""

	'Replace all the <Cdw.ManufacturerLogo> tags with the proper image tag/location
	Do While (DoesPairedTagExist( returnValue, tag) )

		ManufacturerLogoXML	= GetHtmlTagOuter( returnValue, tag, 1 )
		ManufacturerLogo	= ""

			dim xmlDoc, attribute

			Set XmlDoc = Server.CreateObject("MSXML2.DOMDocument")
			xmlDoc.loadXML(ManufacturerLogoXML)
			if (xmlDoc.parseError.errorCode > 0) Then
				XMLError = true
			End If

			if XMLError = false Then
				for each attribute in xmlDoc.documentElement.firstChild.attributes
					if lcase(attribute.NodeName) = "code" Then MFGCode = attribute.text 
					if lcase(attribute.NodeName) = "name" Then MFGName = attribute.text 
					if lcase(attribute.NodeName) = "sitecode" Then SiteCode = attribute.text 
					if lcase(attribute.NodeName) = "size" Then Size = attribute.text 
				next
				ManufacturerLogo = GetManufacturerImageTag(MFGCode, MFGName, Size)
			Else
				ManufacturerLogo = ""
			End If

			Set XmlDoc = nothing
		'This will remove the <Cdw.ManufacturerLogo></Cdw.ManufacturerLogo> tags from the file string.
		returnValue = ReplaceTagInHtml( returnValue, tag, ManufacturerLogo, 0 )
	Loop

	ParseManufacturerLogo = returnValue
end function

'**************************************************************************************
' This function will retrieve a file from cache OR get the content and insert it into the cache.
Function GetContentFileFromCache(ItemCacheKey, FileName)
	Dim returnValue
	Dim LastCachedKey
	Dim DaysCached
	Dim LastCached 
	Dim SecureName

	'If we are on HTTPS, use a different cache so images do not go unsecured on secure server
	If LCase(Request.ServerVariables("HTTPS")) = "on" Then
		SecureName = "_HTTPS"
	Else
		SecureName = ""
	End If

	LastCachedKey = "LastCached" & ItemCacheKey & SecureName
	LastCached = Application(LastCachedKey)

	If ( LastCached = Null Or LastCached = "") Then
		LastCached = Date()
		DaysCached = 1
	Else
		DaysCached = DateDiff("d",Date(), LastCached)
	End If

	returnValue = Null
	returnValue = Application(ItemCacheKey & SecureName)
	if ( Len(returnValue) = 0 OR DaysCached > 0 )  Then
		returnValue	= GetWebcontentIncludeFileString(FileName)

		Application(ItemCacheKey & SecureName) = returnValue
		Application(LastCachedKey) = LastCached
'		Response.Write "Caching " & ItemCacheKey & " at " & LastCached & "<BR>"
'	Else
		'Response.write(ItemCacheKey & " retrieved from Cache<hr />")
	End If

	GetContentFileFromCache = returnValue
End Function
%>
