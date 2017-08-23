<%

'******************************************************************************************
' Constants for will call center locations. 
Const WillCallVhKey 	= "CB4FEC3F712211D4B70B0008C7C55D36"
'Const WillCallOrdKey 	= "CB4FEC40712211D4B70B0008C7C55D36"
Const WillCallLvKey 	= "19CD913075674BBA8BB4C9BAD3041E6A"




'*******************************************************************************************
' TODO: Is anything below here being used? If not, then delete



'****************************************************
' Desc: Build the OrderString for Tax call to the 400
' returns: The OrderString!! Whoohoo!! 
' Pile o Shit alert here...
' Used only in shippingaddresstarget.asp for WillCall Orders

Function Checkout_BuildOrderString(OrderID)
	Dim OrderItems, OrderItemEDC, OrderItemPriceTotal, OrderItemQuantity
	Dim i, returnValue
	returnValue = ""
	'Build the OrderTaxString for the 400 CGI program
	OrderItems = GetCheckoutOrderItems(OrderID)
	If(IsArray(OrderItems)) Then
		For i = LBOUND(OrderItems,2) to UBOUND(OrderItems,2)
			OrderItemEDC 			= OrderItems(cOrderItemEDC,i)
			OrderItemPriceTotal 	= OrderItems(cOrderItemPriceTotal,i)
			'Enhancement request per Martin and 400 team
			OrderItemQuantity		= OrderItems(cOrderItemQuantity,i)
			'The String gets created here
			returnValue = returnValue & "&line" & CStr(i+1) & "=" & OrderItemEDC & "," & OrderItemQuantity & "," & OrderItemPriceTotal
		Next
	End If
	Checkout_BuildOrderString = returnValue
End Function


'***********************************************************
' Desc: This function is being called by shippingaddresstarget.asp
' It will take a TaxString as an arguement which originates from
' the 400. It will then iterate and parse that string and insert
' the values (update) in the Order Items Table with tax info
' This will return the TaxMessage Array containing the values
' you need to serialize in the newCheckout_OrderTaxInfo Table

'Oh and this is just a test.


Function Checkout_WillCallTax(OrderID,TaxString)
	
	'Variables utilized in getting the values from the 400.
	Dim TaxEDC
	Dim TaxRateEDC
	Dim OBOTax
	Dim OBOTaxRate
	Dim ProductCode
	Dim EDCTaxString
	Dim TotalEDCTax
	
	Dim returnValue

	'Initialize TotalEDCTax
	TotalEDCTax = 0

	'Start Formatting the Response
	If Not(IsNull(TaxString)) and Not(IsEmpty(TaxString)) and Len(TaxString) > 0 Then
		TaxString = Replace(TaxString,Chr(10)," ")
		'Lets Split this into its first Array
		Dim ParseArray1
		ParseArray1 = Split(TaxString," " )
		Dim Counter, Counter2
		Dim ParseArray2
		'Loop through the entire name value pair string inside this array
		For Counter = LBOUND(ParseArray1) to UBOUND(ParseArray1)
			'Split the Initial Array into another array
			ParseArray2 = Split(ParseArray1(Counter),"=")
			For Counter2 = LBOUND(ParseArray2) to UBOUND(ParseArray2)
				If (UCASE(ParseArray2(Counter2)) = "TAXHANDLING") Then

					Dim TaxHandlingArray
					Dim TaxHandlingCost,TaxHandlingRate,TaxHandlingOBO,TaxHandlingOBORate
					'Had to increase the counter by one to do this. I hate doing this. I know it breaks every rule...
					TaxHandlingArray 	= Split(ParseArray2(Counter2+1),",")
					TaxHandlingCost 	= TaxHandlingArray(0) 
					TaxHandlingRate 	= TaxHandlingArray(1)
					TaxHandlingOBO		= TaxHandlingArray(2)
					TaxHandlingOBORate	= TaxHandlingArray(3)
				ElseIf (UCASE(ParseArray2(Counter2)) = "TAXFREIGHT") Then

					Dim TaxFreightArray
					Dim TaxFreightCost,TaxFreightRate,TaxFreightOBO,TaxFreightOBORate
					TaxFreightArray 	= Split(ParseArray2(Counter2+1),",")
					TaxFreightCost 		= TaxFreightArray(0)
					TaxFreightRate 		= TaxFreightArray(1)
					TaxFreightOBO		= TaxFreightArray(2)
					TaxFreightOBORate	= TaxFreightArray(3)
				ElseIf (UCASE(ParseArray2(Counter2)) = "TAXINSURANCE") Then

					Dim TaxInsuranceArray
					Dim TaxInsuranceCost,TaxInsuranceRate, TaxInsuranceOBO, TaxInsuranceOBORate
					TaxInsuranceArray 	= Split(ParseArray2(Counter2+1),",")
					TaxInsuranceCost 	= TaxInsuranceArray(0)
					TaxInsuranceRate 	= TaxInsuranceArray(1)
					TaxInsuranceOBO  	= TaxInsuranceArray(2)
					TaxInsuranceOBORate = TaxInsuranceArray(3)
				ElseIf InStr(ParseArray2(Counter2),"taxline") Then
					EDCTaxString = Split(ParseArray2(Counter2+1),",")
					
				'	Response.Write("ProductCode: " & EDCTaxString(0) & "<br>")
				'	Response.Write("TaxEDC: " & EDCTaxString(1) & "<br>")
				'	Response.Write("TaxRateEDC: " & EDCTaxString(2) & "<br>")
				'	Response.Write("OBOTax: " & EDCTaxString(3) & "<br>")
				'	Response.Write("count: " & UBound(EDCTaxString) & "<br>")
				'	Response.End
				
					ProductCode = EDCTaxString(0)
					TaxEDC = EDCTaxString(1)
					TaxRateEDC = EDCTaxString(2)
					
					If UBound(EDCTaxString) > 2 Then
						OBOTax = EDCTaxString(3)
						OBOTaxRate = EDCTaxString(4)
					End If
					
					'Lets Keep a Running Total of the Tax from the SubTotal of Items.
					'Need to check null conditions before doing this.
					If isNull(TaxEDC) or isEmpty(TaxEDC) or Not(IsNumeric(TaxEDC)) Then
						TaxEDC = 0
					End If
					If isNull(OBOTax) or isEmpty(OBOTax) or Not(IsNumeric(OBOTax)) Then
						OBOTax = 0
					End If
					TotalEDCTax = Cdbl(TotalEDCTax) + Cdbl(TaxEDC) + CDbl(OBOTax)
					'Need to call an update here, per EDC.
					Dim UpdateSuccess
					UpdateSuccess = UpdateOrderItemsTaxInfo(OrderID,ProductCode,TaxEDC,TaxRateEDC,OBOTax,OBOTaxRate)
					If Not(UpdateSuccess) Then
						Err.Raise 1,GetThisPage(),"There was an error updating your OrderItems"
					End If
				End If
			Next
		Next
	End If
		'Collect the shipping Charges' tax info into a message array
		' CALL THE INSERT METHOD AFTER YOU INSERT THE SHIPMETHOD
		Dim ShippingTaxMessage(7)
		
		ShippingTaxMessage(TaxInfoOrderID) 			= OrderID
		ShippingTaxMessage(TaxInfoShipTax) 			= TaxFreightCost
		ShippingTaxMessage(TaxInfoShipTaxRate) 		= TaxFreightRate
		ShippingTaxMessage(TaxInfoHandlingTax) 		= TaxHandlingCost
		ShippingTaxMessage(TaxInfoHandlingTaxRate) 	= TaxHandlingRate
		ShippingTaxMessage(TaxInfoInsuranceTax) 	= TaxInsuranceCost
		ShippingTaxMessage(TaxInfoInsuranceTaxRate) = TaxInsuranceRate
		ShippingTaxMessage(TaxInfoTotalEDCTax)		= TotalEDCTax

		returnValue = ShippingTaxMessage
		Checkout_WillCallTax = returnValue
End Function
%>