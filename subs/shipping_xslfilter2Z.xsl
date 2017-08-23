
<xsl:stylesheet version="1.0" exclude-result-prefixes="msxsl local xql" 
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
				xmlns:msxsl="urn:schemas-microsoft-com:xslt" 
				xmlns:local="#local-functions" 
				xmlns:xql="#xql-functions">
	
<xsl:output method="xml" indent="yes" omit-xml-declaration="yes" encoding='UTF-16'/>

	<xsl:template match="@*|/|node()">

		<xsl:copy>
			<xsl:apply-templates select="@*|*|comment()|processing-instruction()|text()"></xsl:apply-templates>
		</xsl:copy>
	</xsl:template>
	
	
	<xsl:template match="rates">
		<xsl:copy>
			<xsl:apply-templates select="carrier[@customerAccount != 'Y' and @code !='WC']">
					<xsl:sort select="number(charge/@totalCharge)" data-type="number" order="ascending" />
			</xsl:apply-templates>
			<!-- AC:  do not include customer accounts on shipping estimator 
			<xsl:apply-templates select="carrier[@customerAccount = 'Y']">
			</xsl:apply-templates>
			-->
		</xsl:copy>
	</xsl:template>
	
	
	<!--remove the Consignee from the XML returned -->
	<xsl:template match="carrier[contains(@name,'Consignee')]">
	</xsl:template>

	<!-- AC:  Dropped Will Call from Shipping Page.  Too many issues with recycling fees, wrong state code, tax etc  -->
	<xsl:template match="carrier[@code = 'WC']">
	</xsl:template>
</xsl:stylesheet>