<%

' ImageDB routines for getting the product, thumbnail and organization images from the database.



Function GetSourceImageRoot()
	Dim DevelopmentMode
	Dim returnValue

	DevelopmentMode = GetSiteDevelopmentMode()

	Select Case DevelopmentMode
		Case glLocalMode
			returnValue = "\\apollo11\webobjects\images\Beta\"
		Case glBetaMode
			returnValue = "\\apollo11\webobjects\images\Beta\"
		Case glPreviewMode
			returnValue = "\\apollo11\webobjects\images\Production\"
		Case glQAMode
			returnValue = "\\apollo11\webobjects\images\Stage\"
		Case glStagingMode
			returnValue = "\\apollo11\webobjects\images\Stage\"
		Case glProductionMode
			returnValue = "\\apollo11\webobjects\images\Production\"
	End Select
	
	GetSourceImageRoot = returnValue
End Function

Function GetDestinationDropDirProductFullImage()
	Dim DevelopmentMode
	Dim returnValue

	DevelopmentMode = GetSiteDevelopmentMode()

	Select Case DevelopmentMode
		Case glLocalMode
			returnValue = "\\apollo11\webobjects\images\products\Beta\drop-full\"
		Case glBetaMode
			returnValue = "\\apollo11\webobjects\images\products\Beta\drop-full\"
		Case glPreviewMode
			returnValue = "\\apollo11\webobjects\images\products\drop-full\"
		Case glQAMode
			returnValue = "\\apollo11\webobjects\images\products\Stage\drop-full\"
		Case glStagingMode
			returnValue = "\\apollo11\webobjects\images\products\Stage\drop-full\"
		Case glProductionMode
			returnValue = "\\apollo11\webobjects\images\products\drop-full\"
	End Select
	
	GetDestinationDropDirProductFullImage = returnValue
	
End Function


Function GetDestinationDropDirProductThumnailImage()
	Dim DevelopmentMode
	Dim returnValue

	DevelopmentMode = GetSiteDevelopmentMode()

	Select Case DevelopmentMode
		Case glLocalMode
			returnValue = "\\apollo11\webobjects\images\products\Beta\drop-thumbs\"
		Case glBetaMode
			returnValue = "\\apollo11\webobjects\images\products\Beta\drop-thumbs\"
		Case glPreviewMode
			returnValue = "\\apollo11\webobjects\images\products\drop-thumbs\"
		Case glQAMode
			returnValue = "\\apollo11\webobjects\images\products\Stage\drop-thumbs\"
		Case glStagingMode
			returnValue = "\\apollo11\webobjects\images\products\Stage\drop-thumbs\"
		Case glProductionMode
			returnValue = "\\apollo11\webobjects\images\products\drop-thumbs\"
	End Select
	
	GetDestinationDropDirProductThumnailImage = returnValue
End Function

Function GetDestinationDropDirOrganizationImage()
	Dim DevelopmentMode
	Dim returnValue

	DevelopmentMode = GetSiteDevelopmentMode()

	Select Case DevelopmentMode
		Case glLocalMode
			returnValue = "\\apollo11\webobjects\images\Organizations\Beta\drop-full\"
		Case glBetaMode
			returnValue = "\\apollo11\webobjects\images\Organizations\Beta\drop-full\"
		Case glPreviewMode
			returnValue = "\\apollo11\webobjects\images\Organizations\drop-full\"
		Case glQAMode
			returnValue = "\\apollo11\webobjects\images\Organizations\Stage\drop-full\"
		Case glStagingMode
			returnValue = "\\apollo11\webobjects\images\Organizations\Stage\drop-full\"
		Case glProductionMode
			returnValue = "\\apollo11\webobjects\images\Organizations\drop-full\"
	End Select
	
	GetDestinationDropDirOrganizationImage = returnValue
End Function


Function GetSourceOldDirProductFullImage()
	Dim DevelopmentMode
	Dim returnValue

	DevelopmentMode = GetSiteDevelopmentMode()
	
	Select Case DevelopmentMode
		Case glLocalMode
			returnValue = "\\apollo11\webobjects\images\products\Beta\full\"
		Case glBetaMode
			returnValue = "\\apollo11\webobjects\images\products\Beta\full\"
		Case glPreviewMode
			returnValue = "\\apollo11\webobjects\images\products\full\"
		Case glQAMode
			returnValue = "\\apollo11\webobjects\images\products\Stage\full\"
		Case glStagingMode
			returnValue = "\\apollo11\webobjects\images\products\Stage\full\"
		Case glProductionMode
			returnValue = "\\apollo11\webobjects\images\products\full\"
	End Select
	
	GetSourceOldDirProductFullImage = returnValue
End Function


Sub CopyPhotoToDropDirectory(sourcePhotoFilename, destinationPhotoFilename)
	Dim objFS
	Set objFS = Server.CreateObject("Scripting.FileSystemObject")
	
	Dim sourcePath, destinationPath
	
	sourcePath = GetSourceOldDirProductFullImage() & sourcePhotoFilename
	destinationPath = GetDestinationDropDirProductFullImage() & destinationPhotoFilename
	
	Response.Write("GetSourceOldDirProductFullImage: " & GetSourceOldDirProductFullImage() & "<br>")
	Response.Write("sourcePath: " & sourcePath & "<br>")
	Response.Write("destinationPath: " & destinationPath & "<br>")	
	Response.Write("objFS.FileExists(sourcePath): " & objFS.FileExists(sourcePath) & "<br>")		

	If objFS.FileExists(sourcePath) = True Then
		Call objFS.CopyFile(sourcePath, destinationPath, true)
	End If
End Sub



'***************************************************************
'	DESCRIPTION:
'		Adds the source and destination image names to the requeue table so that the
'		image upater can pick up the file for reprocessing when the AE user changes
'		the image assignments.
'		Returns the ID of the requeued image entry.

Function Image_AddReprocessQueue(SourceImage, DestinationImage)
	Dim returnValue
	returnValue = Null
		If Not IsEmpty(SourceImage) and Not IsNull(SourceImage) and Len(DestinationImage) > 0 Then
			If Not IsEmpty(DestinationImage) and Not IsNull(DestinationImage) and Len(DestinationImage) > 0 Then
				
                Dim Cmd
				Call OpenSQLConnection(ctCSJobsSalesSQL)
				set Cmd = CreateSQLProcedure(ctCSJobsSalesSQL, "ImageDB.dbo.Image_AddReprocessQueue")
				Cmd.Parameters.Append Cmd.CreateParameter("@SourceImage", adVarChar, adParamInput, 50, SourceImage)				'1
				Cmd.Parameters.Append Cmd.CreateParameter("@DestinationImage", adVarChar, adParamInput, 50, DestinationImage)			'2
				Cmd.Parameters.Append Cmd.CreateParameter("@ReprocessQueueID", adInteger, adParamOutput)						'3
				Call ExecuteSQLProcedure(ctCSJobsSalesSQL, Cmd, False)
				If Cmd.Parameters(0).Value = 0 then
					returnValue = Cmd.Parameters(3).Value
				End If
				Set Cmd = Nothing
				Call CloseSQLConnection(ctReadWrite)
			End If
		End If
	Image_AddReprocessQueue = returnValue
End Function




Function SetUserKey(UserID, UserKey)
	
End Function


%>