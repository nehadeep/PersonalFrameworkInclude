
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
					<xsl:sort select="number(charge/@total)" data-type="number" order="ascending" />
			</xsl:apply-templates>
			
			<xsl:apply-templates select="carrier[@code = 'WC']">
			</xsl:apply-templates>
			
			<xsl:apply-templates select="carrier[@customerAccount = 'Y']">
			</xsl:apply-templates>
		</xsl:copy>
	</xsl:template>
	
	
	<!-- remove the Collect Methods from the above transformation -->
	<!--<xsl:template match="carrier[@customerAccount = 'Y']">
	</xsl:template>-->
	
	<!--remove the Consignee from the XML returned -->
	<xsl:template match="carrier[contains(@name,'Consignee')]">
	</xsl:template>
	<!-- Removed the check for 'Drop' but left 'Consignee'   Andre Coetzee 03/31/2005 -->
	<!--<xsl:template match="carrier[contains(@name,'Consignee') or contains(@name,'Drop')]">
	</xsl:template>-->

	
	<xsl:template match="carrier[charge/@actual = 'N']">
	</xsl:template>
	<!-- PM here: I added the attribute of customerAccount to this new XSLT file because I needed it for the interface! -->
	<xsl:template match="carrier[@code = 'WC']">
		<!-- AC:  Dropped Will Call from Shipping Page.  Too many issues with recycling fees, wrong state code, tax etc  -->
		<!--
		<xsl:copy>
			<xsl:attribute name="code">01</xsl:attribute>
			<xsl:attribute name="name">Will Call Pickup, Vernon Hills</xsl:attribute>
			<xsl:attribute name="customerAccount">N</xsl:attribute>
			<xsl:element name="description">Pick up merchandise at the Vernon Hills, IL showroom</xsl:element>
			<xsl:apply-templates select="message"></xsl:apply-templates>
			<xsl:apply-templates select="charge"></xsl:apply-templates>
			<xsl:apply-templates select="eta"></xsl:apply-templates>
			<xsl:apply-templates select="@cost"></xsl:apply-templates>
			<xsl:apply-templates select="@customerAccount"></xsl:apply-templates>
		</xsl:copy>
		<xsl:copy>
			<xsl:attribute name="code">07</xsl:attribute>
			<xsl:attribute name="name">Will Call Pickup, Las Vegas</xsl:attribute>
			<xsl:attribute name="customerAccount">N</xsl:attribute>
			<xsl:element name="description">Pick up merchandise at the Las Vegas, NV warehouse</xsl:element>
			<xsl:apply-templates select="message"></xsl:apply-templates>
			<xsl:apply-templates select="charge"></xsl:apply-templates>
			<xsl:apply-templates select="eta"></xsl:apply-templates>
			<xsl:apply-templates select="@cost"></xsl:apply-templates>
			<xsl:apply-templates select="@customerAccount"></xsl:apply-templates>
		</xsl:copy>
			-->
	</xsl:template>
</xsl:stylesheet>