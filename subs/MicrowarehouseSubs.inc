<%
'***************************************************************
'	DESCRIPTION:
'		Returns true or false depending on whether a customer number has old MicroWarehouse orders.

Function DoesCustomerNumberHaveMWOrders(CompanyCode, CustomerNumber)
	Dim returnValue, sqlStatement, SoapRequestXml, SoapResponseXml, DataSetXml
	returnValue = false

	SoapRequestXml = (_
		"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
		"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
		"  <soap:Body>" & vbNewLine &_
		"	<DoesCustomerNumberHaveOrders xmlns=""http://tempuri.org/"">" & vbNewLine &_
		"	  <companyCode>" & CompanyCode & "</companyCode>" & vbNewLine &_
		"	  <customerNumber>" & CustomerNumber & "</customerNumber>" & vbNewLine &_
		"	</DoesCustomerNumberHaveOrders>" & vbNewLine &_
		"  </soap:Body>" & vbNewLine &_
		"</soap:Envelope>" & vbNewLine )
	SoapResponseXml = Soap_GetResponseXml( "http://" & GetSiteName(webservice_cdw_com) & "/Cdw/BusinessObjects/Applications/MicroWarehouse/OrderHistoryWebService.asmx", SoapRequestXml, "http://tempuri.org/DoesCustomerNumberHaveOrders" )
	'	returnValue = SoapResponseXml
'	response.write soapresponsexml
	If Not IsNull(SoapResponseXml) And Not IsEmpty(SoapResponseXml) And Len(Trim(SoapResponseXml)) > 0 Then
'		response.write Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//DoesCustomerNumberHaveOrdersResult" )
		returnValue = Soap_StringToBoolean( Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//DoesCustomerNumberHaveOrdersResult" ) )
	End If

	DoesCustomerNumberHaveMWOrders = returnValue
End Function

'***************************************************************
'	DESCRIPTION:
'		Returns true or false depending on whether a customer number has old MicroWarehouse quotes.

Function DoesCustomerNumberHaveMWQuotes(CompanyCode, CustomerNumber)
	Dim returnValue, sqlStatement, SoapRequestXml, SoapResponseXml, DataSetXml
	returnValue = false

	SoapRequestXml = (_
		"<?xml version=""1.0"" encoding=""utf-8""?>" & vbNewLine &_
		"<soap:Envelope xmlns:xsi=""http://www.w3.org/2001/XMLSchema-instance"" xmlns:xsd=""http://www.w3.org/2001/XMLSchema"" xmlns:soap=""http://schemas.xmlsoap.org/soap/envelope/"">" & vbNewLine &_
		"  <soap:Body>" & vbNewLine &_
		"	<DoesCustomerNumberHaveQuotes xmlns=""http://tempuri.org/"">" & vbNewLine &_
		"	  <companyCode>" & CompanyCode & "</companyCode>" & vbNewLine &_
		"	  <customerNumber>" & CustomerNumber & "</customerNumber>" & vbNewLine &_
		"	</DoesCustomerNumberHaveQuotes>" & vbNewLine &_
		"  </soap:Body>" & vbNewLine &_
		"</soap:Envelope>" & vbNewLine )

	SoapResponseXml = Soap_GetResponseXml( "http://" & GetSiteName(webservice_cdw_com) & "/Cdw/BusinessObjects/Applications/MicroWarehouse/OrderHistoryWebService.asmx", SoapRequestXml, "http://tempuri.org/DoesCustomerNumberHaveQuotes" )
	'	returnValue = SoapResponseXml
'	response.write soapresponsexml
	If Not IsNull(SoapResponseXml) And Not IsEmpty(SoapResponseXml) And Len(Trim(SoapResponseXml)) > 0 Then
'		response.write Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//DoesCustomerNumberHaveQuotesResult" )
		returnValue = Soap_StringToBoolean( Soap_GetSingleValueFromResponseXml( SoapResponseXml, "//DoesCustomerNumberHaveQuotesResult" ) )
	End If

	DoesCustomerNumberHaveMWQuotes = returnValue
End Function
%>