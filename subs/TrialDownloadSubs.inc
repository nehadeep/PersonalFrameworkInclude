<%
Function GetComputerAssociatesInstallationInstructions(FormID)
	Dim returnValue, sqlStatement, SoapRequestXml, SoapResponseXml, DataSetXml
	returnValue = null

	SoapRequestXml = (_
		"<?xml version='1.0' encoding='utf-8'?>" & vbNewLine &_
		"<soap:Envelope xmlns:xsi='http://www.w3.org/2001/XMLSchema-instance' xmlns:xsd='http://www.w3.org/2001/XMLSchema' xmlns:soap='http://schemas.xmlsoap.org/soap/envelope/'>" & vbNewLine &_
		"	<soap:Body>" & vbNewLine &_
		"		<GetInstallationInstructions xmlns='http://ca.com/'>" & vbNewLine &_
		"			<UserName>CDW</UserName>" & vbNewLine &_
		"			<Password>WSTrials</Password>" & vbNewLine &_
		"			<FormID>" & FormID & "</FormID>" & vbNewLine &_
		"		</GetInstallationInstructions>" & vbNewLine &_
		"	</soap:Body>" & vbNewLine &_
		"</soap:Envelope>" & vbNewLine)

	SoapResponseXml = Soap_GetResponseXml( "http://www3.ca.com/webservices/trials/installationinstructions.asmx", SoapRequestXml, "http://ca.com/GetInstallationInstructions" )

	dim columnNameArray(3)
	columnNameArray(0) = "FormName"
	columnNameArray(1) = "FormOverView"
	columnNameArray(2) = "FormEmailMessage"

	If Not IsNull(SoapResponseXml) And Not IsEmpty(SoapResponseXml) And Len(Trim(SoapResponseXml)) > 0 Then
		returnValue = Soap_GetArrayOfRecordsFromResponseXml( SoapResponseXml, "//Installation_x0020_Instructions", columnNameArray )
	End If

	GetComputerAssociatesInstallationInstructions = returnValue
End Function
%>