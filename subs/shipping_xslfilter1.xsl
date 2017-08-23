
<xsl:stylesheet version="1.0" exclude-result-prefixes="msxsl local xql" 
				xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
				xmlns:msxsl="urn:schemas-microsoft-com:xslt" 
				xmlns:local="#local-functions" 
				xmlns:xql="#xql-functions">

<xsl:output method="xml" indent="yes" omit-xml-declaration="yes"/>
<xsl:template match="@*|/|node()">
		<xsl:copy>
			<xsl:apply-templates select="@*|*|comment()|processing-instruction()|text()"></xsl:apply-templates>
		</xsl:copy>
</xsl:template>

<xsl:template match="charge">
	<xsl:copy>
		<xsl:apply-templates select="@*|*|comment()|processing-instruction()|text()"></xsl:apply-templates>
		<xsl:attribute name="total">
			<xsl:value-of select="number(@shipping) + number(@total_handling) + number(@insurance)"/>
		</xsl:attribute>
	</xsl:copy>
</xsl:template>
</xsl:stylesheet>
