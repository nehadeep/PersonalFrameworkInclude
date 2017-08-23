<%
Dim ErrorStyle
	ErrorStyle = "style='background-color:#FFFF00'"
'**************************************************************************************************
'********************************************************************
'*	DrawMainProductFeature AND DrawMainLifestyleFeature				*
'* 	 both take 8 Parameters:										*
'*		1.  Header:			Approximately 20 Characters	(1 line)	*
'*		2.  SubHeader:		Approximately 50 Characters (2 lines)	*
'*		3.  Content:		Approximately 92 Characters(3 lines)	*
'*		4.  Price Callout:	Approximately 15 Characters	(1 line)	*
'*		5.  Price:			Price for the feature					*
'*		6.  Link:			The URL to the destination.				*
'*		7.  MFG:			MFG Code for the SBB Channel and logo.	*
'*		8.  Image:			EDC for product image/custom graphic 	*
'*		9.  ContentID:		The ID for the feature for use w/RO		*
'********************************************************************
Sub DrawMainLifestyleFeature(mfHeader, mfSubheader, mfContent, mfPriceCall, mfPrice, mfDisclaimer, mfLink, mfMFG, mfImage, mfContentID)
	Dim HeadStyle, SubHeadStyle, ContentStyle, PriceStyle, DisplayValue, DisclamerStyle
		HeadStyle		= ""
		SubHeadStyle	= ""
		ContentStyle	= ""
		PriceStyle		= ""
		DisclamerStyle	= ""

	If mfContentID = "FeatureOne" Then
		DisplayValue = "display:block;"
	Else
		DisplayValue = "display:none;"
	End If

	If glDevelopmentMode <> glProductionMode Then
		If Len(mfHeader) > 20 Then HeadStyle = ErrorStyle End If
		If Len(mfSubheader) > 50 Then SubHeadStyle = ErrorStyle End If
		If Len(mfContent) > 85 Then ContentStyle = ErrorStyle End If		'85 is to show that the message MAY be to long.
		If Len(mfPriceCall) > 15 Then PriceStyle = ErrorStyle End If
		If Len(mfDisclaimer) > 18 Then DisclamerStyle = ErrorStyle End If
	End If

	Response.write(_
		"<div id='" & mfContentID & "' style='" & DisplayValue & " width: 386px;'>" & VbCrLf & _
			"<table cellpadding='0' cellspacing='0' id=' & mfContentID & ' border='0' style='background: url(" & GetImagePath(mfImage) & ") no-repeat;' height='186' width='386'>" & _
				"<tr>" & _
					"<td colspan='3' width='386'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='386' height='13' border='0' alt='' /></td>" & _
				"</tr>" & _
				"<tr>" & _
					"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='190' height='1' border='0' alt='' /></td>" &_
					"<td width='181'>" & _
						"<table cellpadding='0' cellspacing='0' width='100%' border='0'>" & _
							"<tr>" & _
								"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='173' border='0' alt='' /></td>" & _
								"<td valign='top'>" & _
									"<span class='bodyHeadLarge' " & HeadStyle & ">" & mfHeader & "</span><br />" & _
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='3' border='0' alt='' /><br />" & _
									"<span class='btsb' " & SubHeadStyle & ">" & mfSubheader & "</span><br />" & _
									"<span class='bts' " & ContentStyle & ">" & mfContent & "</span><br />" & _
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='5' border='0' alt='' /><br />" & _
									"<table cellpadding='0' cellspacing='0'>" & _
										"<tr>" & _
											"<td " & PriceStyle & ">"  )
												If Not(IsNull(mfPriceCall)) AND Len(mfPriceCall) > 0 Then
													Response.write(_
														"<span class='btsb'>" & mfPriceCall & "</span><br />" & _
														"<span class='bodyHeadLargeRed'>" & mfPrice & "</span>" )
														If Not(IsNull(mfDisclaimer)) AND Len(mfDisclaimer) > 0 Then
															Response.write(_
																"<br />" & _
																"<span class='sst6'>" & mfDisclaimer & "</span>")
														End If
												Else
													Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='120' height='5' border='0' alt='' />")
												End If

										Response.write(_
											"</td>" & _
											"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='5' border='0' alt='' /></td>" & _
											"<td valign='bottom'><a href='" & mfLink & "'><img src='" & GetImagePath("/cdw/homepage/gui/feature_moreinfo_btn.gif") & "' border='0' alt='More Info' /></a></td>" & _
										"</tr>" & _
									"</table>" & _
									"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='180' height='4' border='0' alt='' /><br />" )

									If Not(IsNull(mfMFG)) AND Len(mfMFG) > 0 Then
										Response.write(_
										"<div style='text-align:right;'>" & _
											"<a href='" & glSBBVendorURL & "?mfg=" & mfMFG & "'>" & _
												GetManufacturerImageTag(mfMFG, "Brand Channel", mfgImageSmall) & _
											"</a>"  &_
										"</div>")
									End If

							Response.write(_
								"</td>" & _
							"</tr>" & _
						"</table>" & _
					"</td>" & _
					"<td width='10'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' alt='' /></td>" & _
				"</tr>" & _
			"</table>" & vbCrLf  & _
		"</div>" & VbCrLf )
End Sub

'**************************************************************************************************
Sub DrawMainProductFeature(mfHeader, mfSubheader, mfContent, mfPriceCall, mfPrice, mfDisclaimer, mfLink, mfMFG, mfImage, mfContentID)
	Dim HeadStyle, SubHeadStyle, ContentStyle, PriceStyle, DisplayValue, DisclamerStyle
		HeadStyle		= ""
		SubHeadStyle	= ""
		ContentStyle	= ""
		PriceStyle		= ""
		DisclamerStyle	= ""

	If mfContentID = "FeatureOne" Then
		DisplayValue = "display:block;"
	Else
		DisplayValue = "display:none;"
	End If

	If glDevelopmentMode <> glProductionMode Then
		If Len(mfHeader) > 20 Then HeadStyle = ErrorStyle End If
		If Len(mfSubheader) > 50 Then SubHeadStyle = ErrorStyle End If
		If Len(mfContent) > 85 Then ContentStyle = ErrorStyle End If
		If Len(mfPriceCall) > 15 Then PriceStyle = ErrorStyle End If
		If Len(mfDisclaimer) > 18 Then DisclamerStyle = ErrorStyle End If
	End If

	Response.write(_
		"<div id='" & mfContentID & "' style='" & DisplayValue & " width: 386px;'>" & VbCrLf & _
			"<table cellpadding='0' cellspacing='0' border='0' width='386'>" & _
				"<tr>" & _
					"<td width='10'><img src='" & GetImagePath("/cdw/homepage/gui/feature_left.gif") & "' border='0' alt='' /></td>" & _
					"<td>" & _
						"<table cellpadding='0' cellspacing='0' width='366' border='0'>" & _
							"<tr>" & _
								"<td><img src='" & GetImagePath("/cdw/homepage/gui/feature_span_top_orange.gif") & "' border='0' width='100%' height='3' alt='' /></td>" & _
							"</tr>" & _
							"<tr>" & _
								"<td>" & _
									"<table cellpadding='0' cellspacing='0' width='100%' border='0'>" & _
										"<tr>" & _
											"<td><a href='" & mfLink & "'>" & GetProductImageTagByCode(mfImage, 1) & "</a></td>" & _
											"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='180' border='0' alt=''></td>" & _
											"<td valign='top'>" & _
												"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='10' border='0' alt='' /><br />" & _
												"<span class='bodyHeadLarge' " & HeadStyle & ">" & mfHeader & "</span><br />" & _
												"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='181' height='3' border='0' alt='' /><br />" & _
												"<span class='btsb' " & SubHeadStyle & ">" & mfSubheader & "</span><br />" & _
												"<span class='bts' " & ContentStyle & ">" & mfContent & "</span><br />" & _
												"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='3' border='0' alt='' /><br />" & _
												"<table cellpadding='0' cellspacing='0'>" & _
													"<tr>" & _
														"<td " & PriceStyle & ">"  )
															If Not(IsNull(mfPriceCall)) AND Len(mfPriceCall) > 0 Then
																Response.write(_
																	"<span class='btsb'>" & mfPriceCall & "</span><br />" & _
																	"<span class='bodyHeadLargeRed'>" & mfPrice & "</span>" )

																	If Not(IsNull(mfDisclaimer)) AND Len(mfDisclaimer) > 0 Then
																	Response.write(_
																		"<br />" & _
																		"<span class='sst6' " & DisclamerStyle & ">" & mfDisclaimer & "</span>")
																	End If
															Else
																Response.write("<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='120' height='5' border='0' alt='' />")
															End If

													Response.write(_
														"</td>" & _
														"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='5' border='0' alt='' /></td>" & _
														"<td valign='bottom'><a href='" & mfLink & "'><img src='" & GetImagePath("/cdw/homepage/gui/feature_moreinfo_btn.gif") & "' border='0' alt='More Info' /></a></td>" & _
													"</tr>" & _
												"</table>" & _
												"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='180' height='7' border='0' alt='' /><br />" )

												If Not(IsNull(mfMFG)) AND Len(mfMFG) > 0 Then
													Response.write(_
													"<div style='text-align:right;'>" & _
														"<a href='" & glSBBVendorURL & "?mfg=" & mfMFG & "'>" & _
															GetManufacturerImageTag(mfMFG, "Brand Channel", mfgImageSmall) & _
														"</a>"  &_
													"</div>")
												End If

										Response.write(_
											"</td>" & _
											"<td width='10'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' alt='' /></td>" & _
										"</tr>" & _
									"</table>" & _
								"</td>" & _
							"</tr>" & _
							"<tr>" & _
								"<td><img src='" & GetImagePath("/cdw/homepage/gui/feature_span_bottom_orange.gif") & "' border='0' width='100%' height='3' alt='' /></td>" & _
							"</tr>" & _
						"</table>" & _
					"</td>" & _
				"</tr>" & _
			"</table>" & vbCrLf  & _
		"</div>" & vbCrLf)
End Sub

'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'This will draw out one of the headlines in the upper righthand corner of the homepage.

Sub WriteHeadlineWithRollover(HLTitle, HLText, HLLink, HLContentID)
	Dim HeadStyle, ContentStyle, JavaScriptTag, bgImage, fontColor
		HeadStyle		= ""
		ContentStyle	= ""
		JavaScriptTag	= ""
		bgImage			= "/cdw/homepage/gui/arrow_inactive.gif"
		fontColor		= ""
	If glDevelopmentMode <> glProductionMode Then
		If Len(HLTitle) > 25 Then HeadStyle = ErrorStyle End If
		If Len(HLText) > 80 Then ContentStyle = ErrorStyle End If
	End If

		If HLContentID = "FeatureOne" Then 
			JavaScriptTag	= "onMouseOver='change(""FeatureOne"", ""show""); change(""FeatureTwo"", ""hide""); change(""FeatureThree"", ""hide""); SelectHeadline(""FeatureOneHL""); DeselectHeadline(""FeatureTwoHL""); DeselectHeadline(""FeatureThreeHL"");'"
			bgImage			= "/cdw/homepage/gui/arrow_active.gif"
			fontColor		= "color:#CC0000;"
		ElseIf HLContentID = "FeatureTwo" Then 
			JavaScriptTag	= "onMouseOver='change(""FeatureOne"", ""hide""); change(""FeatureTwo"", ""show""); change(""FeatureThree"", ""hide""); DeselectHeadline(""FeatureOneHL""); SelectHeadline(""FeatureTwoHL""); DeselectHeadline(""FeatureThreeHL"");'"
		ElseIf HLContentID = "FeatureThree" Then 
			JavaScriptTag	= "onMouseOver='change(""FeatureOne"", ""hide""); change(""FeatureTwo"", ""hide""); change(""FeatureThree"", ""show""); DeselectHeadline(""FeatureOneHL""); DeselectHeadline(""FeatureTwoHL""); SelectHeadline(""FeatureThreeHL""); '"
		End If

	Response.write(_
		"<tr>" & _
			"<td colspan='4'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='12' border='0' alt=''></td>" & _
		"</tr>"  & vbCrLf &_
		"<tr>" & _
			"<td valign='top' bgcolor='#F6F6F6'>" & VbCrLf & _
				"<div class='btsb'" & HeadStyle & " id='" & HLContentID & "HL' onclick='MM_goToURL(""parent"", """ & HLLink & """);' " & JavaScriptTag & " style='" & fontColor & "background: url(" & GetImagePath(bgImage) & ") no-repeat; cursor: hand; background-color: #F6F6F6;' >" & VbCrLf & _
					"<img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='10' border='0' alt=''>" & _
					HLTitle & _
					"<table border='0' cellspacing='0' cellpadding='0' bgcolor='#F6F6F6'>" & _
						"<tr>" & _
							"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='10' border='0' alt=''></td>" & _
							"<td width='100%' class='bts' " & ContentStyle & "><a href='" & HLLink & "' class='btslink'>" & HLText & "</a></td>" &_
						"</tr>" & _
					"</table>" &_
				"</div>" & VbCrLf & _
			"</td>" & VbCrLf & _
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='1' border='0' alt=''></td>" & _
		"</tr>" & vbCrLf)
End Sub
'**************************************************************************************************

'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'This will draw out one of the headlines in the upper righthand corner of the homepage.

Sub WriteHeadline(HLTitle, HLText, HLLink)
	Dim HeadStyle, ContentStyle
		HeadStyle		= ""
		ContentStyle	= ""

	If glDevelopmentMode <> glProductionMode Then
		If Len(HLTitle) > 25 Then HeadStyle = ErrorStyle End If
		If Len(HLText) > 80 Then ContentStyle = ErrorStyle End If
	End If

	Response.write(_
		"<tr>" & _
			"<td colspan='4'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='12' border='0' alt=''></td>" & _
		"</tr>"  & vbCrLf &_
		"<tr>" & _
			"<td valign='top'><img src='" & GetImagePath("/cdw/homepage/gui/tri.gif") & "' border='0' alt='' vspace='4'></td>" & _
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='5' border='0' alt=''></td>" & _
			"<td width='100%'>" & _
				"<a href='" & HLLink & "' class='blbRed' " & HeadStyle & ">" & HLTitle & "</a><br/>" & _
				"<a href='" & HLLink & "' class='btslink3' " & ContentStyle & ">" & HLText & "</a>" & _
			"</td>" & _
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='1' border='0' alt=''></td>" & _
		"</tr>" & vbCrLf)
End Sub

'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'This will draw out one of the smartbuys on the left column of the homepage.

'SmartBuy: Limit to 2 lines, 75 charaters
Sub WriteSmartBuySpot(sbCallout, sbText, sbLink, sbImage)
	Dim SBStyle
		SBStyle		= ""

	If glDevelopmentMode <> glProductionMode Then
		If Len(sbCallout) + Len(sbText) > 75 Then SBStyle = ErrorStyle End If
	End If

	Response.write(_
		"<tr>" & _
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' alt=''></td>" & _
			"<td valign='top'><img src='" & GetImagePath("/cdw/homepage/gui/tri.gif") & "' border='0' alt='' vspace='3'></td>" & _
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='1' border='0' alt=''></td>" & _
			"<td width='100%' " & SBStyle & ">" & _
				"<a href='" & sbLink & "' class='SidebarLink' onMouseOver=""MM_swapImage('sbMainImage','','" & sbImage & "',0); MM_swapImage('sbMainLink','','" & sbLink & "',0)"">" & _
					"<span style='font-weight: bold;'>" & sbCallout & "</span> " & sbText &  _
				"</a>" & _
			"</td>" & _
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='7' height='1' border='0' alt=''></td>" & _
		"</tr>" & VbCrLf & _
		"<tr>" & _
			"<td colspan='4'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='5' border='0' alt=''></td>" & _
		"</tr>" & VbCrLf)
End Sub

'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'This will draw out the initial image for the smart buys.

Sub WriteSmartBuyInitialImage(sbImage)
	Response.write(_
		"<table cellpadding='0' cellspacing='0' border='0' width='100%'>" & _
			"<tr>" & _
				"<td><img src='" & GetImagePath("/cdw/homepage/gui/sb_photoframe_left.gif") & "' border='0' alt=''></td>" & _
				"<td valign='top'>" & _
					"<table cellpadding='0' cellspacing='0' width='100%'>" & _
						"<tr>" & _
							"<td colspan='2'><img src='" & GetImagePath("/cdw/homepage/gui/sb_photoframe_top.gif") & "' border='0' width='100%' height='4' alt=''></td>" & _
						"</tr>" & _
						"<tr>" & _
							"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='1' height='84' border='0' alt=''></td>" & _
							"<td><img src='" & sbImage & "' name='sbMainImage' height='80' width='80' border='0' alt=''></td>" & _
						"</tr>" & _
						"<tr>" & _
							"<td colspan='2'><img src='" & GetImagePath("/cdw/homepage/gui/sb_photoframe_top.gif") & "' border='0' width='100%' alt=''></td>" & _
						"</tr>" & _
					"</table>" & _
				"</td>" & _
				"<td><img src='" & GetImagePath("/cdw/homepage/gui/sb_photoframe_right.gif") & "' border='0' alt=''></td>" & _
			"</tr>" & _
		"</table>" & vbCrLf)
End Sub
'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'This will draw out one of the Product spots with a product image in the right column of the homepage.

Sub WriteProductSpotWithImage(psHeadline, psContent, psLink, psEDC)
	Dim HeadStyle, ContentStyle
		HeadStyle		= ""
		ContentStyle	= ""

	If glDevelopmentMode <> glProductionMode Then
		If Len(psHeadline) > 35 Then HeadStyle = ErrorStyle End If
		If Len(psContent) > 128 Then ContentStyle = ErrorStyle End If
	End If

	Dim ProductImageTag
		ProductImageTag = GetProductImageTagbyCode(psEDC, prProductImageThumb)

	Response.write(_
		"<tr>" &_
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' alt=''></td>" &_
			"<td valign='top' " & HeadStyle & "><a href='" & psLink & "' class='btbRed' style='text-transform: uppercase; text-decoration:none;'>" & psHeadline & "</a></td>" &_
		"</tr>" & vbCrLf &_
		"<tr>" &_
			"<td colspan='2'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='3' border='0' alt=''></td>" &_
		"</tr>" & vbCrLf &_
		"<tr>" &_
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' alt=''></td>" &_
			"<td valign='top'>" &_
				"<table cellpadding='0' cellspacing='0' border='0' width='100%'>" &_
					"<tr>" &_
						"<td width='100%' valign='top'>" &_
							"<table cellpadding='0' cellspacing='0' border='0' width='100%'>" &_
								"<tr>" &_
									"<td class='bts' " & ContentStyle & ">" & psContent & "</td>" &_
								"</tr>" &_
								"<tr>" &_
									"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='6' border='0' alt=''></td>" &_
								"</tr>" &_
								"<tr>" &_
									"<td valign='top'><a href='" & psLink & "'><img src='" & GetImagePath("/cdw/homepage/gui/btn_moreinfo.gif") & "' border='0' alt=''></a></td>" &_
								"</tr>" &_
							"</table>" & vbCrLf &_
						"</td>" &_
						"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='10' height='1' border='0' alt=''></td>" &_
						"<td valign='top'><a href='" & psLink & "'>" & ProductImageTag & "</a></td>" &_
						"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='1' border='0' alt=''></td>" &_
					"</tr>" &_
				"</table>" & vbCrLf &_
			"</td>" &_
		"</tr>" & vbCrLf)
End Sub
'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'This will draw out one of the Product spots without a product image in the right column of the homepage.

Sub WriteProductSpotWithoutImage(psHeadline, psContent, psLink)
	Dim HeadStyle, ContentStyle
		HeadStyle		= ""
		ContentStyle	= ""

	If glDevelopmentMode <> glProductionMode Then
		If Len(psHeadline) > 40 Then HeadStyle = ErrorStyle End If
		If Len(psContent) > 88 Then ContentStyle = ErrorStyle End If
	End If

	Response.write(_
		"<tr>" &_
			"<td colspan='4'><a href='" & psLink & "' class='blbRed' " & HeadStyle & ">" & psHeadline & "</a></td>" &_
		"</tr>" &_
		"<tr>" &_
			"<td class='bts' valign='top' " & ContentStyle & ">" & psContent & "</td>" &_
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='5' height='1' border='0' alt=''></td>" &_
			"<td valign='bottom'><a href='" & psLink & "'><img src='" & GetImagePath("/cdw/homepage/gui/btn_moreinfo_sm.gif") & "' border='0' alt=''></a></td>" &_
			"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='7' height='1' border='0' alt=''></td>" &_
		"</tr>" &_
		"<tr>" &_
			"<td colspan='4'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='5' border='0' alt=''></td>" &_
		"</tr>" & vbCrLf)
End Sub
'**************************************************************************************************
'Constants for the branding spot image types
	const bsCustomImage		= 0
	const bsProductImage	= 1
	const bsMFGImage		= 2
'**************************************************************************************************
'Created on 1/5/05 by Paul Haan
'************************************************************
'*	WriteHeadline takes 3 Parameters:						*
'*		1.  Title:  Approximately 25 Characters				*
'*		2.  Text:	Approximately 70 Characters				*
'*		3.  Link:	The full URL to the destination.		*
'************************************************************
'This will draw out one of the branding spots in the left column of the homepage.

Sub WriteBrandSpot(bsHeadline, bsContent, bsLink, bsImage, bsImageType)
	Dim ImageTag
	Dim HeadStyle, ContentStyle
		HeadStyle		= ""
		ContentStyle	= ""

	If glDevelopmentMode <> glProductionMode Then
		If Len(bsHeadline) > 28 Then HeadStyle = ErrorStyle End If
		If Len(bsContent) > 64 Then ContentStyle = ErrorStyle End If
	End If

	If bsImageType = bsProductImage Then
		Dim ImageSrc
		ImageSrc	= GetProductImageLocationbyCode(bsImage,prProductImageThumb)
		ImageTag	= "<img width='80' height='80' border='0' src='" & ImageSrc & "' alt='Product Photo:  " & HTMLEncode(bsImage) & "'>"

		'ImageTag	= GetProductImageTagbyCode(bsImage, prProductImageThumb)
	Elseif bsImageType = bsMFGImage Then
		ImageTag	= GetManufactureImageTag(bsImage, bsHeadline, mfgImageSmall)
	Else
		ImageTag	= "<img src='" & GetImagePath(bsImage) & "' alt='" & bsHeadline & "' border='0' />"
	End If

	Response.write(_
		"<tr>" & _
			"<td width='193' valign='top'>" & _
				"<table cellpadding='0' cellspacing='0' border='0' width='193'>" & _
					"<tr>" & _
						"<td class='btb' " & HeadStyle & "><a href='" & bsLink & "' style='text-decoration:none;'>" & bsHeadline & "</a></td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td class='bts' " & ContentStyle & ">" & bsContent & "</td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='5' border='0' alt=''></td>" & _
					"</tr>" & _
					"<tr>" & _
						"<td><a href='" & bsLink & "'><img src='" & GetImagePath("/cdw/homepage/gui/btn_moreinfo.gif") & "' border='0' alt=''></a></td>" & _
					"</tr>" & _
				"</table>" & _
			"</td>" & _
			"<td width='3'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='3' height='1' border='0' alt=''></td>" & _
			"<td width='80' align='right'><a href='" & bsLink & "'>" & Trim(ImageTag) & "</a></td>" & _
		"</tr>" & _
		"<tr>" & _
			"<td colspan='3'><img src='" & GetImagePath("/global/pixels/none.gif") & "' width='100%' height='12' border='0' alt=''></td>" & _
		"</tr>" & vbCrLf)
End Sub
'**************************************************************************************************

'Used to draw the nugget style features...
sub DrawSiteNugget(title, OptionalST, subtitle, text, altTag, EDC, URL, startdate, enddate)

	Dim ImageTag, URLPath, ImageSrc

	ImageSrc	= GetProductImageLocationbyCode(EDC, prProductImage60x)
	ImageTag = 	"<img border='0' src='" & ImageSrc & "' alt='" & title  & "' align='left' hspace='0' vspace='0' />"

	'make sure the dates are ok
	If (CDate(StartDate) < Date()) AND (Date() < CDate(EndDate)) Then
			Response.Write(_
				"<table border='0' cellpadding='0' cellspacing='0' width='145'>" & _
					"<tr>" & _
						"<td align='left' width='6' height='105'>" & _
							"<img src='" & GetImagePath("/macwarehouse/gui/nugget_left.gif") & "' height='105' width='6' />" & _
						"</td>" & _
						"<td width='132' height='105' style='border-bottom: 1px solid #574128; border-top: 1px solid #574128; '>" & _
							"<table border='0' cellpadding='0' cellspacing='0' width='132' height='103'>" & _
								"<tr>" & _
									"<td rowspan='2' height='103'><img src='" & GetImagePath("/global/pixels/none.gif") & "' height='103' width='1' /></td>" & _
									"<td colspan='2' class='NGheader' height='18'>" & _
										"<a href='" & URL & "' class='NGheader' title='" & altTag & "'>" & Title  & "</a><br />")

										If Not(IsNull(OptionalST)) and Not(IsEmpty(OptionalST)) and Len(OptionalST) > 0 Then
											Response.write("<a href='" & URL & "' class='NGheader' title='" & altTag & "'>" & OptionalST & "</a><br />")
										End If

			Response.write(_
											"<a href='" & URL & "' class='NGsubtitle' title='" & altTag & "'>" & subtitle & "</a><br />" & _
									"</td>" & _
								"</tr>" & _
								"<tr>" & _
									"<td valign='top' class='NGbody'>" &_
										"<img src='" & GetImagePath("/global/pixels/none.gif") & "' height='1' width='68'><br />" & _
										"<a href='" & URL & "' class='NGbody' title='" & altTag & "'>" & _
										 text & "</a><br />" & _
										"<a href='" & URL & "'>" & _
											"<img src='" & GetImagePath("/macWarehouse/gui/moreinfo.gif") & "' border='0' alt='More Info: " & altTag & "' vspace='3'/>" & _
										"</a>" & _
									"</td>" & _
									"<td width='65' valign='top' style='padding-left: 5px;'>" & _
										"<a href='" & URL & "' class='sstLink'>" & ImageTag & "</a>" & _
									"</td>" & _
								"</tr>" & _
							"</table>" &_
						"</td>" & _
						"<td align='right' width='6' height='105'>" & _
							"<img src='" & GetImagePath("/macwarehouse/gui/nugget_right.gif") & "' height='105' width='6' />" & _
						"</td>" & _
					"</tr>" & _
				"</table>")
	End if
end Sub

	'Modified History

	'Date: Nov.9.2001
	'By: David Tzau
	'Description: Assumes global.inc, sqlconn.inc included.  Retrive CompanyCode depending on the site.
	'			  Each company will have it's own pricing model.   GetProductPriceAdvertised has changed

	Const exRV_Success = 0
	Const exRV_EOF = 1
	Const exRV_InvalidEntry = 2

	Const exTRV_Success = 0
	Const exTRV_EOF = 1

	Const XMLFileLocation = "/webobjects/assets/exclusives/bestsellers/"

'**************************************************************************************************************************
'	Render main exclusives

	Function renderExclusive(Sequence, ShowSeeMore, xmlSet, baseURL, extraQueryString)
		Dim ReturnValue, ProductID
		Dim xmlExclusive, EDC, Thumbnail, ExclusiveName, Body, ManuName, ManuCode, ManuHasChannel, CallOut
		Dim actionURL, SeeMoreURL, AddToCartURL, SeeMoreLabel
		Dim userID, userKey, OrganizationID, OrganizationKey
		Dim OrganizationProductPricing, OrganizationProductPrice
		Dim ProductPriceName, ProductPrice
		Dim baseActionURL, baseSeeMoreMfgURL , baseAddToCartURL
		Dim padQueryString
		ReturnValue = exRV_Success
		
		'Site specific information
		Dim CompanyID
	
		'Array to store Product Pricing
		Dim arProductPrice
			
		'determine companyid
		CompanyID = GetCompanyID(GetThisProperty())
				
		If(UserIsLoggedIn()) Then
			userKey = getUserKey()
			userID = getUserID(userKey)
		End If
		If(UserIsIdentified()) Then
			'OrganizationKey = GetUserOrganizationKey()
			OrganizationID = GetOrganizationIdFromUser() 'GetOrganizationID(OrganizationKey)
		End If

		if IsNull(extraQueryString) then
			padQueryString = ""
		else
			padQueryString = extraQueryString & "&"
		end if

		Set xmlExclusive = xmlSet.selectSingleNode("/exclusiveSet/exclusive[@sequence='" & Sequence & "']")
		If TypeName(xmlExclusive) = "Nothing" Then
			ReturnValue = exRV_EOF
		Else
			EDC = CLng(xmlExclusive.selectSingleNode("@instance").nodeValue)
			CallOut = ""
			ThumbNail = LCase(Trim(xmlExclusive.selectSingleNode("thumbnail/@href").nodeValue))
			ExclusiveName = Trim(xmlExclusive.selectSingleNode("name").Text)
			Body = Trim(xmlExclusive.selectSingleNode("body").Text)
			ManuName = Trim(xmlExclusive.selectSingleNode("manufacturer").Text)
			ManuCode = Trim(xmlExclusive.selectSingleNode("manufacturer/@code").nodeValue)
			
			if IsNull(baseURL) then
				baseActionURL = glProductURL & "?" & padQueryString & "ProductID="
				baseAddToCartURL = glAddCartItemURL & "?" & padQueryString & "EDC="
				baseSeeMoreMfgURL = "/shop/tools/sbb/vendor.asp?" & padQueryString & "MFG=" & ManuCode
			else
				baseActionURL = baseURL & "/products/default.aspx?" & padQueryString & "ProductID="
				baseAddToCartURL = baseURL & "/checkout/addcartitem.asp?" & padQueryString & "EDC="
				baseSeeMoreMfgURL = baseURL & "/tools/sbb/vendor.asp?" & padQueryString & "mfg="& ManuCode
			end if

			ProductID = GetProductIDByCode(EDC)

			' Organization special price
			OrganizationProductPricing = GetOrganizationProductPricing(OrganizationID, ProductID)
			If(Not(IsNull(OrganizationProductPricing))) Then
				OrganizationProductPrice = GetOrganizationProductPrice(OrganizationProductPricing)
				If(Not(IsNull(OrganizationProductPrice))) Then
					ProductPriceName	= OrganizationProductPrice(eaOrganizationProductPriceName)
					ProductPrice		= OrganizationProductPrice(eaOrganizationProductPrice)
				End If
			Else
				arProductPrice = GetProductPrices(ProductID, CompanyID)
				If isArray(arProductPrice) Then
		  			ProductPrice = arProductPrice(prProductPriceAdvertised)
		  		End If
			End If
		
				Set xmlExclusive = Nothing
				
				actionURL = baseActionURL & ProductID
				AddToCartURL = baseAddToCartURL & EDC

				If ShowSeeMore Then
					SeeMoreLabel = "More " & ManuName & "..."
					SeeMoreURL = baseSeeMoreMfgURL
				Else
					SeeMoreLabel = ""
					SeeMoreURL = ""
				End If

				'Write out exclusive. Thumbnail contains the entire <IMG SRC=...> tag for the image
				Response.Write "<A HREF='" & HTMLEncode(actionURL) & "'> " & ThumbNail & "</A><br>"

				if len(CallOut) > 0 then
					Response.Write "<span class='bts' style='COLOR:#993333'>" & HTMLEncode(CallOut) & "</span><BR>"
				end if
						
				Response.Write "<A class='ExclusiveLinkBold' HREF='" & HTMLEncode(actionURL) & "'>" & HTMLEncode(ExclusiveName) & "</A><BR>"

				If Len(trim(Body)) > 0 then
					Response.Write "<span class='bts6'>" & HTMLEncode(Body) & "</span><br>"
				End IF

				Response.Write "<span class='numExclusive' title='" & HtmlEncode(ProductPriceName) & "'> " & HTMLEncode(FormatMoney(ProductPrice))

				If(Not(IsEmpty(ProductPriceName)) and Not(IsNull(ProductPriceName))) Then
					Response.Write(_
						"*")
				End If

				Response.Write "</B></span><br>"

				Response.Write "<span class='bts'><A HREF='" & HTMLEncode(AddToCartURL) &"'>Add to Cart</A></span><br>"

				if len(SeeMoreURL) > 0 then
					Response.Write "<span class='bts'><A HREF='" & HTMLEncode(SeeMoreURL) & "'>" & HTMLEncode(SeeMoreLabel) & "</a></span><br>"
				end if
		End If
		renderExclusive = ReturnValue
	End Function

	Function renderExclusivesTable(NumberOfRows, LocationType, LocationInstance, ShowSeeMore, baseURL, extraQueryString)
		Dim xmlSet
		Dim SequenceNumber, iCol, iRow, renderLoopContinue, renderExclusiveReturnValue
		Dim ReturnValue, Http, XMLPath
		ReturnValue = exTRV_EOF

		SequenceNumber = 0

		On Error Resume Next
		Set Http = CreateServerXMLHTTP()
		Set xmlSet = CreateDomDocument()
		XMLPath = GetWebObjectPath(XMLFileLocation) & LocationType & LocationInstance & ".xml"
		' Kinda of a hack having to do with the css1100 not being able to get to webobjects.cdw.com from the same interface.
		XMLPath = Replace(XMLPath, "webobjects.cdw.com", "webobjint.wip.cdw.com")
		Http.open "GET", XMLPath, False
		Http.send

		xmlSet.loadXML(Http.ResponseText)
		Set Http = Nothing

		Response.Write("<table border=0 cellpadding=0 cellspacing=0 summary='Table for Featured Products'>")
		For iRow = 0 to NumberOfRows - 1
			Response.Write("<tr><td colspan='3'><img src='" & GetImagePath("/global/pixels/none.gif") & "' border=0 height=20 width=20 alt='Spacing Image'><BR></td></tr>")
			Response.Write("<tr>")
			For iCol = 0 to 2
				Response.Write("<td width='30%' valign='top'>")
				renderLoopContinue = True
				Do While renderLoopContinue = True
					renderExclusiveReturnValue = renderExclusive(SequenceNumber, ShowSeeMore, xmlSet, baseURL, extraQueryString)
					Select Case renderExclusiveReturnValue
					Case exRV_Success
						renderLoopContinue = False
						ReturnValue = exTRV_Success
					Case exRV_EOF
						renderLoopContinue = False
						ReturnValue = exTRV_EOF
					Case exRV_InvalidEntry
						renderLoopContinue = True
					End Select
					SequenceNumber = SequenceNumber + 1
				Loop
				Response.Write("</td>")
				If iCol < 2 Then
					Response.Write("<td class='bts'>&nbsp;&nbsp;&nbsp;</td>")
				End If
			Next
			Response.Write("</tr>")
		Next

		Response.Write("</table>")
		If ReturnValue = exTRV_Success Then
			If TypeName(xmlSet.selectSingleNode("/exclusiveSet/exclusive[@sequence='" & SequenceNumber & "']")) = "Nothing" Then
				ReturnValue = exTRV_EOF
			End If
		End If
		Set xmlSet = Nothing
		Err.Clear
		renderExclusivesTable = ReturnValue
	End Function

'**************************************************************************************************************************
'	Vertical exclusives for the category pages

	Function renderVerticalExclusive(Sequence, ShowSeeMore, xmlSet, baseURL, extraQueryString)
		Dim ReturnValue, ProductID
		Dim xmlExclusive, EDC, Thumbnail, ExclusiveName, Body, ManuName, ManuCode, ManuHasChannel, CallOut
		Dim actionURL, SeeMoreURL, AddToCartURL, SeeMoreLabel
		Dim userID, userKey, OrganizationID, OrganizationKey
		Dim OrganizationProductPricing, OrganizationProductPrice
		Dim ProductPriceName, ProductPrice
		Dim baseActionURL, baseSeeMoreMfgURL, baseAddToCartURL
		Dim padQueryString
		ReturnValue = exRV_Success

		'Site specific information
		Dim CompanyID

		'Array to store Product Pricing
		Dim arProductPrice

		'determine companyid
		CompanyID = GetCompanyID(GetThisProperty())

		If(UserIsLoggedIn()) Then
			userKey = getUserKey()
			userID = getUserID(userKey)
		End If
		If(UserIsIdentified()) Then
			'OrganizationKey = GetUserOrganizationKey()
			OrganizationID = GetOrganizationIdFromUser() 'GetOrganizationID(OrganizationKey)
		End If

		if IsNull(extraQueryString) then
			padQueryString = ""
		else
			padQueryString = extraQueryString & "&"
		end if

		Set xmlExclusive = xmlSet.selectSingleNode("/exclusiveSet/exclusive[@sequence='" & Sequence & "']")

		If TypeName(xmlExclusive) = "Nothing" Then
			ReturnValue = exRV_EOF
		Else

			EDC = CLng(xmlExclusive.selectSingleNode("@instance").nodeValue)
			CallOut = ""
			ThumbNail = LCase(Trim(xmlExclusive.selectSingleNode("thumbnail/@href").nodeValue))
			ExclusiveName = Trim(xmlExclusive.selectSingleNode("name").Text)

			if len(Trim(xmlExclusive.selectSingleNode("body").Text)) > 149 then
				Body = Left(Trim(xmlExclusive.selectSingleNode("body").Text),148) & "..."
			else
				Body = Trim(xmlExclusive.selectSingleNode("body").Text)
			end if

			ManuName = Trim(xmlExclusive.selectSingleNode("manufacturer").Text)
			ManuCode = Trim(xmlExclusive.selectSingleNode("manufacturer/@code").nodeValue)
			
			if IsNull(baseURL) then
				baseActionURL = glProductURL & "?" & padQueryString & "ProductID="
				baseAddToCartURL = glAddCartItemURL & "?" & padQueryString & "EDC="
				baseSeeMoreMfgURL = "/shop/tools/sbb/vendor.asp?" & padQueryString & "MFG=" & ManuCode
			else
				baseActionURL = baseURL & "/products/default.aspx?" & padQueryString & "ProductID="
				baseAddToCartURL = baseURL & "/checkout/addcartitem.asp?" & padQueryString & "EDC="
				baseSeeMoreMfgURL = baseURL & "/tools/sbb/vendor.asp?" & padQueryString & "mfg="& ManuCode
			end if

			ProductID = GetProductIDByCode(EDC)

			' Organization special price
			OrganizationProductPricing = GetOrganizationProductPricing(OrganizationID, ProductID)
			If(Not(IsNull(OrganizationProductPricing))) Then
				OrganizationProductPrice = GetOrganizationProductPrice(OrganizationProductPricing)
				If(Not(IsNull(OrganizationProductPrice))) Then
					ProductPriceName	= OrganizationProductPrice(eaOrganizationProductPriceName)
					ProductPrice		= OrganizationProductPrice(eaOrganizationProductPrice)
				End If
			Else
				arProductPrice = GetProductPrices(ProductID, CompanyID)
				If isArray(arProductPrice) Then
		  			ProductPrice = arProductPrice(prProductPriceAdvertised)
		  		End If
			End If

			Set xmlExclusive = Nothing

			If LCase(Trim(ManuHasChannel)) = "y" Then
			    ManuHasChannel = True
			Else
			    ManuHasChannel = False
			End If

			actionURL = baseActionURL & ProductID
			AddToCartURL = baseAddToCartURL & EDC

			If ShowSeeMore Then
				SeeMoreLabel = "More " & ManuName & "..."
				SeeMoreURL = baseSeeMoreMfgURL
			Else
				SeeMoreLabel = ""
				SeeMoreURL = ""
			End If

			'Write out exclusive. Thumbnail contains the entire <IMG SRC=...> tag for the image
			Response.write "<table border=0 cellpadding=0 cellspacing=0 summary='table for individual product'>"
			Response.write "<tr><td>"
			Response.Write "<A HREF='" & actionURL & "'>" & ThumbNail & "</A><br>"
			Response.write "</td>"
			Response.write "<td>&nbsp;&nbsp;</td>"
			Response.write "<td>"
			if len(CallOut) > 0 then
				Response.Write("<span class='btsRed'>" & HTMLEncode(CallOut) & "</span><BR>")
			end if

			Response.Write("<A class='ExclusiveLinkBold' HREF='" & HTMLEncode(actionURL) & "'>" & HTMLEncode(ExclusiveName) & "</A><BR>")

			If Len(trim(Body)) > 0 then
				Response.Write("<span class='bts6'>" & HTMLEncode(Body) & "</span><br>")
			End IF

			Response.Write("<span class='numExclusive' title='" & HtmlEncode(ProductPriceName) & "'> " & FormatMoney(ProductPrice))

			If(Not(IsEmpty(ProductPriceName)) and Not(IsNull(ProductPriceName))) Then
				Response.Write("*")
			End If

			Response.Write("</span><br>")

			if len(SeeMoreURL) > 0 then
				Response.Write("<span class='bts'><A HREF='" & HTMLEncode(SeeMoreURL) & "'>" & HTMLEncode(SeeMoreLabel) & "</a></span><br>")
			end if
			Response.write("</td></tr></table>")
		end if
		renderVerticalExclusive = ReturnValue
	End Function

'**************************************************************************************************************************
	Function renderVerticalExclusivesTable(NumberOfRows, LocationType, LocationInstance, ShowSeeMore, baseURL, extraQueryString)
		Dim xmlSet
		Dim SequenceNumber, iCol, iRow, renderLoopContinue, renderVerticalExclusiveReturnValue
		Dim ReturnValue, Http, XMLPath
		ReturnValue = exTRV_EOF

		SequenceNumber = 0

		On Error Resume Next
		Set Http = CreateServerXMLHTTP()
		Set xmlSet = CreateDomDocument()
		XMLPath = GetWebObjectPath(XMLFileLocation) & LocationType & LocationInstance & ".xml"
		' Kinda of a hack having to do with the css1100 not being able to get to webobjects.cdw.com from the same interface.
		XMLPath = Replace(XMLPath, "webobjects.cdw.com", "webobjint.wip.cdw.com")
		Http.open "GET", XMLPath, False
		Http.send

		xmlSet.loadXML(Http.ResponseText)
		Set Http = Nothing

		Response.Write("<table border=0 cellpadding=0 cellspacing=0 summary='Table for Featured Products'>")
		For iRow = 0 to NumberOfRows - 1
			Response.Write("<tr><td colspan='3'><img src='" & GetImagePath("/global/pixels/none.gif") & "' border=0 height=20 width=20 alt='Spacing Image'><BR></td></tr>")
			Response.Write("<tr>")
				Response.Write("<td width='100%' valign='top'>")
				renderLoopContinue = True
				Do While renderLoopContinue = True

					renderVerticalExclusiveReturnValue = renderVerticalExclusive(SequenceNumber, ShowSeeMore, xmlSet, baseURL, extraQueryString)

					Select Case renderVerticalExclusiveReturnValue
					Case exRV_Success
						renderLoopContinue = False
						ReturnValue = exTRV_Success
					Case exRV_EOF
						renderLoopContinue = False
						ReturnValue = exTRV_EOF
					Case exRV_InvalidEntry
						renderLoopContinue = True
					End Select

					SequenceNumber = SequenceNumber + 1
				Loop
				Response.Write("</td>")
			Response.Write("</tr>")
		Next

		Response.Write("</table>")
		If ReturnValue = exTRV_Success Then
			If TypeName(xmlSet.selectSingleNode("/exclusiveSet/exclusive[@sequence='" & SequenceNumber & "']")) = "Nothing" Then
				ReturnValue = exTRV_EOF
			End If
		End If
		Set xmlSet = Nothing
		Err.Clear
		renderVerticalExclusivesTable = ReturnValue
	End Function

'**************************************************************************************************************************
function DoesExclusiveExist(mfgCode)
	dim returnValue, FileName

	returnValue = false
	FileName = GetWebObjectPath(XMLFileLocation) & "mfg" &  mfgCode & ".xml"

	if HTTPFileExists(FileName) then
		returnvalue = true
	else
		returnvalue = false
	end if

	DoesExclusiveExist = returnvalue
end function
%>