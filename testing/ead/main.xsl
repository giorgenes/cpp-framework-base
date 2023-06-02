<?xml version="1.0" encoding="ISO-8859-1"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:output method='html' version='1.0' encoding='UTF-8' indent='yes'/>
	
	<xsl:template name="html_head">
		<head>
			<link rel="stylesheet" type="text/css" href="style.css" />
			<script type="text/javascript" src="main.js" />
		</head>
	</xsl:template>
	
	<xsl:template name="logo_block">
		<img src="http://stat.livejournal.com/xc/logocombined.gif" />
	</xsl:template>

	<xsl:template name="not_logged_block">
		<a href="" onclick="return show_login_div()">click to loggin</a>
	</xsl:template>
		
	<xsl:template name="login_form_block">
		<form>
			<input type="text" id="user" />
			<input type="submit" id="b1" />
		</form>
	</xsl:template>

	
	<xsl:template name="content_block">
		<xsl:apply-templates select="//course" />
	</xsl:template>

	<xsl:template match="/">
		<html>
			<xsl:call-template name="html_head"/>
			<body>
				<div name="logo">
					<xsl:call-template name="logo_block"/>
				</div>
				<div id="not_logged">
					<xsl:call-template name="not_logged_block"/>
				</div>
				<div id="login_form">
					<xsl:call-template name="login_form_block"/>
				</div>

				<div name="content">
					<xsl:call-template name="content_block"/>
				</div>

			</body>
		</html>
	</xsl:template>

	<xsl:template match="course">
		<xsl:variable name="lk" select="id" />
		<a href="{$lk}.xml"><xsl:value-of select="desc"/></a><br/>
	</xsl:template>

</xsl:stylesheet>

