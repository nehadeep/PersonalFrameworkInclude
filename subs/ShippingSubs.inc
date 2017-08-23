<%

'***********************************************************
' Description: Determines whether the shipping code is any of the USPS Mail types.
'			   Values are taken from AS400 running this query
'			   "select * from oep87ul1".  This should be in a database, getting values
'			   from AS400, but the shipping page is already too slow to make another
'			   database call.
' Returns: True/False
' By: David Tzau
' Date: Dec.5.2001
'***********************************************************
Const usFirstClassMailLetter		= "US"
Const usExpressMail					= "EM"
Const usPriorityMail				= "PM"
Const usPriorityAir					= "PA"
Const usFirstClassMail				= "FC"
Const usExpressMailCollect			= "EX"
Const usInternationalExpressMail	= "IE"
Const usInternationPriorityMail		= "IP"

Function isUSPSMailType(shippingCode)
	Dim returnValue
	
	returnValue = False
	
	shippingCode = UCASE(shippingCode)
	
	If shippingCode = usFirstClassMailLetter or _
		shippingCode = usExpressmail or _
		shippingCode = usPriorityMail or _
		shippingCode = usPriorityAir or _
		shippingCode = usFirstClassMail or _
		shippingCode = usExpressmailCollect or _
		shippingCode = usInternationalExpressMail or _
		shippingCode = usInternationPriorityMail Then
			returnValue = true
	End If
		
	isUSPSMailType = returnValue
End Function


'**************************************************************
'	DESCRIPTION:
'		Validates the ShippingCustomerAccount against different rules.
'		Will to a security/error redirect if an error was found.
'	RETURN:
'		Modified ShippingCustomerAccount
Function ValidateShippingCustomerAccount(OrderID, ShippingCarrierCode, ShippingCustomerAccount, SecurityRedirectPage)
	Dim returnValue, i
	returnValue = Trim(ShippingCustomerAccount)

	' Check for Airborne
	Dim AirborneCollect(4)
	AirborneCollect(0) = "AB"
	AirborneCollect(1) = "AO"
	AirborneCollect(2) = "AT"
	AirborneCollect(3) = "AE"
	AirborneCollect(4) = "AI" 'Added by SJE on 1/7/2004 (I am assuming that this should have been here and follow the same rules as above)
	For i = LBOUND(AirborneCollect) to UBOUND(AirborneCollect)
		If AirborneCollect(i) = Trim(ShippingCarrierCode) Then
			'Validate the CarrierAccount #
			If Not(Len(returnValue) > 5) or Not(Len(returnValue) <= 9) or Not(IsNumeric(returnValue)) Then
				'Handle Errors here with the proper Redirect
				Call SecurityRedirect( errInvalid, erShippingMethodInvalidAirborne, SecurityRedirectPage )
			End If
		End If
	Next

	' Check for DHL
	Dim DHLCollect(1)
	DHLCollect(0) = "DX"
	For i = LBOUND(DHLCollect) to UBOUND(DHLCollect)
		If DHLCollect(i) = Trim(ShippingCarrierCode) Then
			'New Rule: Check to see what country its shipping to
			Dim ISOCountryCode
			Dim OrderShippingAddressArray
			OrderShippingAddressArray = GetCheckoutOrderShippingAddress(OrderID)
			ISOCountryCode = OrderShippingAddressArray(cShipToISOCountryCode)
			If (Not IsNull(ISOCountryCode)) and (Not IsEmpty(ISOCountryCode)) Then
				If Trim(ISOCountryCode) <> "US" Then
					If Left(returnValue,2) <> "96" or (Not Len(returnValue) = 9) or (Not IsNumeric(returnValue)) Then
						Call SecurityRedirect( errInvalid, erShippingMethodInvalidDHL, SecurityRedirectPage )
					End If
				End If
			End If
			If (Not Len(returnValue) = 9) or (Not IsNumeric(returnValue)) Then
				'Handle Errors here with the proper Redirect
				Call SecurityRedirect( errInvalid, erShippingMethodInvalidDHL, SecurityRedirectPage )
			End If
		End If
	Next

	' Check for FedEx!
	Dim FedExCollect(8)
	FedExCollect(0) = "X1"
	FedExCollect(1) = "XJ"
	FedExCollect(2) = "XI"
	FedExCollect(3) = "X5"
	FedExCollect(4) = "X7"
	FedExCollect(5) = "X3"
	FedExCollect(6) = "XE"
	FedExCollect(7) = "XF"
	FedExCollect(8) = "RC"
	For i = LBOUND(FedExCollect) to UBOUND(FedExCollect)
		If FedExCollect(i) = Trim(ShippingCarrierCode) Then
			Dim ValidFedEx
			'Time to Validate Fed Ex Numbers:
			'Strip the dashes
			returnValue = Replace(returnValue,"-","")
			'Start checking the length of the string. Moved this
			'From the API Call to outside, cause length is different for RC
			' updated 4/15/2003 by Salim. New Fedex rules allow 9+digit number for ground collect.
			If Trim(ShippingCarrierCode) = "RC" And Len(returnValue) = 6 or Len(returnValue) <=7 Then
				ValidFedEx = True
			Else
				If Len(returnValue) >= 9 Then
					Response.Write "returnValue"
					ValidFedEx = ValidateFedExAccountNumber(returnValue)
					If Not(ValidFedEx) Then
						'Handle Errors here with the proper Redirect
						Call SecurityRedirect( errInvalid, erShippingMethodInvalidFedEx, SecurityRedirectPage )
  					End If
				Else
					'Handle Errors here with the proper Redirect
					Call SecurityRedirect( errInvalid, erShippingMethodInvalidFedEx, SecurityRedirectPage )
				End If
			End If
		End If
	Next

	' Check for UPS
	Dim UPSCollect(7)
	UPSCollect(0) = "P2"
	UPSCollect(1) = "UC"
	UPSCollect(2) = "PG"
	UPSCollect(3) = "P1"
	UPSCollect(4) = "UJ"
	UPSCollect(5) = "P3"
	UPSCollect(6) = "PL"
	UPSCollect(7) = "PR"
	For i = LBOUND(UPSCollect) to UBOUND(UPSCollect)
		If UPSCollect(i) = Trim(ShippingCarrierCode) Then
			'Validate the CarrierAccount #
			returnValue = Replace(returnValue,"-","")
			'Trim off the 1Z if its there in the first two chars
			If Ucase(Left(returnValue,2)) = "1Z" Then
				returnValue = Replace(returnValue,Left(returnValue,2),"")
			End If
			If (Not Len(returnValue) >= 6) or (Not Len(returnValue) <= 15) Or Not(validParagraph(returnValue)) Then
				Call SecurityRedirect( errInvalid, erShippingMethodInvalidUPS, SecurityRedirectPage )
			End If
		End If
	Next

	' Check for Express
	Dim ExpressCollect(1)
	ExpressCollect(0) = "EX"
	For i = LBOUND(ExpressCollect) to UBOUND(ExpressCollect)
		If ExpressCollect(i) = Trim(ShippingCarrierCode) Then
			'Validate the CarrierAccount #
			If (Not Len(returnValue) >= 6) or (Not Len(returnValue) <= 9) or (Not IsNumeric(returnValue)) Then
				'Handle Errors here with the proper Redirect
				Call SecurityRedirect( errInvalid, erShippingMethodInvalidExpress, SecurityRedirectPage )
			End If
		End If
	Next

	ValidateShippingCustomerAccount = returnValue
End Function


%>
