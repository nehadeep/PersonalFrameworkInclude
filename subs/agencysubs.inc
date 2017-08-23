<%					
	'	DESCRIPTION:
	'		Agency Page for CDWG.  Draws the 6 feature products for the specific agency
	'		(Federal, State/Local, Education)
	'	ASSUMPTIONS:
	'		Must include /webcontent/includes/hubs/cdwg/agencies/AgencyFeatureProducts.inc
	'		This file stores the feature EDC's to display on the agency pages.
	'		/global/subs/product2subs.inc
	'		/global/subs/eaccountsubs.inc

	Const agencyFederal	= 1
	Const agencyStateLocal	= 2
	Const agencyEducation	= 3
	Const ProductURL = "/shop/products/default.asp?EDC="

	Sub DrawAgencyFeatureProducts(agencyType)
	'------------------------------------------------
	' Description: Draws the feature products for the agency
	'------------------------------------------------
		Dim ProductCount	  'number of feature products to display
		Dim FeatureProducts  'array to store featured product edc's
		Dim x				  'index counter
		Dim ProductID, Product, arProductPrice, ProductPrice, ProductImageTag  'product vars
		Dim CompanyID 	'site specific info
		DIM FeaturePromotionImage, FeaturePromotionImageText, FeaturePromotionLinkID

		'retrieve companyid
		CompanyID = GetCompanyID(GetThisProperty())

		'Determine agencyType and set parameters
		Select Case (agencyType)
			Case agencyFederal
				ProductCount				= UBound(FederalFeaturedProducts)
				FeatureProducts				= FederalFeaturedProducts
				FeaturePromotionImage		= FederalFeaturePromotionImage
				FeaturePromotionLinkID		= FederalFeaturePromotionLinkID
				FeaturePromotionImageText	= FederalFeaturePromotionImageText
			Case agencyStateLocal
				ProductCount				= UBound(StateFeaturedProducts)
				FeatureProducts				= StateFeaturedProducts
				FeaturePromotionImage		= StateFeaturePromotionImage
				FeaturePromotionLinkID		= StateFeaturePromotionLinkID
				FeaturePromotionImageText	= StateFeaturePromotionImageText
			Case agencyEducation		
				ProductCount				= UBound(EducationFeaturedProducts)
				FeatureProducts				= EducationFeaturedProducts
				FeaturePromotionImage		= EducationFeaturePromotionImage
				FeaturePromotionLinkID		= EducationFeaturePromotionLinkID
				FeaturePromotionImageText	= EducationFeaturePromotionImageText
		End Select

		'Display Feature Products for Agency
		Response.Write ("<table width='100%' border='0' cellspacing='0' cellpadding='3'>")

			if Not(isNull(FeaturePromotionImage)) and Not(IsEmpty(FeaturePromotionImage)) and Len(FeaturePromotionImage) > 0 Then
				Response.write("<tr><td colspan='2'>")

				if Not(isNull(FeaturePromotionLinkID)) and Not(IsEmpty(FeaturePromotionLinkID)) and Len(FeaturePromotionLinkID) > 0 Then
					Response.write("<a href='http://go.cdw.com/?id=" & FeaturePromotionLinkID & "'>")
				End If
				Response.write("<img src='" & FeaturePromotionImage & "' alt='" & FeaturePromotionImageText & "' border='0' />")
				if Not(isNull(FeaturePromotionLinkID)) and Not(IsEmpty(FeaturePromotionLinkID)) and Len(FeaturePromotionLinkID) > 0 Then
					Response.write("</a>")
				End If

				Response.write(_
						"</td>" &_
					"</tr>" & _
					"<tr>" &_
						"<td colspan='2' align='center' valign='top'><img src='" & GetImagePath("/cdwg/agency/dot_line.gif") & "' alt=''></td>" &_
					"</tr>")
			End If

			For x = 1 to ProductCount
				'get the productID from the EDC
				ProductID = GetProductIDByCode(FeatureProducts(x, agencyEDC))

				'get product information
				Product = GetProduct(ProductID)

				'retrieve product advertised price
				arProductPrice = GetProductPrices(ProductID, CompanyID)
				If isArray(arProductPrice) Then
					ProductPrice = arProductPrice(prProductPriceAdvertised)
				Else
					ProductPrice = Null
				End If

				If Not(IsNull(Product)) And isArray(Product) Then
					'get product image tag
					ProductImageTag = GetProductImageTagbyCode(Product(prProductCode), prProductImageThumb)

					Response.write(_
						"<tr>"  &_
							"<td width='110' align='center' valign='top'>" &_
								"<a href='/r.asp?n=" & HTMLEncode(FeatureProducts(x, agencyEDCRedirect)) & "&cdwEDC=" & HTMLEncode(Trim(Product(prProductCode))) & "'>" & ProductImageTag & "</a>" &_
							"</td>" &_
							"<td valign='top' class='sst' width='140'>" &_
								"<a href='/r.asp?n=" & HTMLEncode(FeatureProducts(x, agencyEDCRedirect)) & "&cdwEDC=" & HTMLEncode(Trim(Product(prProductCode))) & "' class='blbRed'>" & HTMLEncode(Trim(Product(prProductFriendlyName))) &"</a><br />" &_
								HTMLEncode(Trim(Product(prProductFriendlyDescription))) & "<br />" &_
								"<span class='btsb'>" & HTMLEncode(FormatMoney(ProductPrice)) & "</span><br />" &_
							"</td>" &_
						"</tr>")

					' Do not display the dotted line gif for the last product
					If x <> ProductCount Then
						Response.Write(_
							"<tr>" &_
								"<td colspan='2' align='center' valign='top'><img src='" & GetImagePath("/cdwg/agency/dot_line.gif") & "' alt=''></td>" &_
							"</tr>")
					End If
				End If
			Next 
				
		Response.write("</table>")
	End Sub	

	Sub DrawLinksOfInterest(agencyType)
	'------------------------------------------------
	' Description: Draws the links of interest for the agency
	'------------------------------------------------
		Dim FeaturedLinks  '2d array storing featured links, gif and url info
		Dim LinkCount, x

		'Determine agencyType and set parameters
		Select Case (agencyType)
			Case agencyFederal
				LinkCount = UBound(FederalFeaturedLinks)
				FeaturedLinks = FederalFeaturedLinks
			Case agencyStateLocal
				LinkCount = UBound(StateFeaturedLinks)
				FeaturedLinks = StateFeaturedLinks
			Case agencyEducation		
				LinkCount = UBound(EducationFeaturedLinks)
				FeaturedLinks= EducationFeaturedLinks
		End Select

		Response.Write ("<table width='100%' border='0' cellspacing='1' cellpadding='1'>")

		For x = 1 to LinkCount
			Response.Write("<tr>" &_
						"<td bgcolor='#FFFFFF' class='sst3' valign='top'><img src='" & GetImagePath(FeaturedLinks(x,FeaturedURLImage)) & "' vspace='6' alt='" & FeaturedLinks(x, FeaturedURLImageALTTag) & "'><br />" &_
						"<a href='" & HTMLEncode(FeaturedLinks(x, FeaturedURLLink)) & "'>" & FeaturedLinks(x, FeaturedDescription) & "</a><br />" &_
					"</tr>" &_
					"<tr>" &_
						"<td bgcolor='#CCCCCC' class='sst3' valign='top'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' alt=''></td>" &_
					"</tr>")
		Next
		Response.Write ("</table>")
	End Sub
%>