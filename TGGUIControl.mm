//
//  TGGUIControl.m
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "TGGUIControl.h"


#include "znt.h"
#include "libs/tga.h"

@implementation TGGUIControl

- (id)init
{
	if (self = [super init])
	{
		NSLog (@"====== ProcLab :: Loading ======");
		[self init_prepareText];
	}
	return self;
}

- (void)dealloc
{
	[super dealloc];
}

- (void)awakeFromNib
{
	[CBOperation selectItemAtIndex:0];
	[self renderFinal:nil];
	[self renderTemp:nil];
}

// Reset all temporary textures
- (void)resetTemp:(id)sender
{
	for (int i=0; i<TMP_TEXTURES; i++)
	{
		CTextGen *tmptex = tg_temptext[i];

		tmptex->Init();
		COneTextGen txtaux;
		tmptex->dtex.insert(tmptex->dtex.end(), txtaux);
		tmptex->dtexsize = tmptex->dtex.size();
		tmptex->Regenerate();
		NSLog(@"Re-initializing temporary text #%d [Size: %lu]... done!!",i, tmptex->dtex.size());
	}
	[self renderTemp:nil];
}

// Prepare texture data
- (void)init_prepareText
{
	int i;
	for (i=0; i<TMP_TEXTURES; i++)
	{
		tg_temptext[i] = new CTextGen();
	}
	[self resetTemp:nil];
	tg_final = new CTextGen();
	tg_final->Init();
	NSLog(@"Init Final text done!!");
}

// open texture file
- (IBAction) openZNTfile:(id)sender
{
	
	// "Standard" open file panel
	NSArray *fileTypes = [NSArray arrayWithObjects:@"znt", nil];
	
	int i;
	// Create the File Open Panel class.
	NSOpenPanel* oPanel = [NSOpenPanel openPanel];
	//[oPanel setParentWindow:mainWindow];	// Define the parent of our dialog
	[oPanel setFloatingPanel:NO];				// When we move our parent window, the dialog moves with it
	
	[oPanel setCanChooseDirectories:NO];		// Disable the selection of directories in the dialog.
	[oPanel setCanChooseFiles:YES];				// Enable the selection of files in the dialog.
	[oPanel setCanCreateDirectories:YES];		// Enable the creation of directories in the dialog
	[oPanel setAllowsMultipleSelection:NO];		// Allow multiple files selection
	[oPanel setAlphaValue:0.95];				// Alpha value
	[oPanel setTitle:@"Select a file to open"];
    [oPanel setAllowedFileTypes:fileTypes ];
	
	// Display the dialog.  If the OK button was pressed,
	// process the files.
	if ( [oPanel runModal] == NSOKButton )
	{
		// Get an array containing the full filenames of all
		// files and directories selected.
		NSArray* files = [oPanel URLs];
		
		// Loop through all the files and process them.
		for( i = 0; i < [files count]; i++ )
		{
            NSString *fileName = [[files objectAtIndex:i] path];
			const char *file = [fileName fileSystemRepresentation];
			NSLog(@"Open File: %s", file);
			ZNT fd((char*)file, *tg_final);
			char msg;
			msg = fd.LoadFile(true);
			
			if (msg == -1)
				NSLog(@"Err: openZNTfile --> Could not read the file. File version is not valid.");
			if (msg == -2)
				NSLog(@"Err: openZNTfile --> Could not read the file. File is not valid.");
			
			[self renderFinal:nil];	
		}
	}
}



// save texture file
- (IBAction) saveZNTfile:(id)sender
{
    NSArray *fileTypes = [NSArray arrayWithObjects:@"znt", nil];

	// Create the File Save Panel class.
	NSSavePanel* sPanel = [NSSavePanel savePanel];
	//[oPanel setParentWindow:mainWindow];	// Define the parent of our dialog
	[sPanel setFloatingPanel:NO];				// When we move our parent window, the dialog moves with it
	[sPanel setAllowedFileTypes:fileTypes];
	[sPanel setCanCreateDirectories:YES];		// Enable the creation of directories in the dialog
	[sPanel setAlphaValue:0.95];				// Alpha value
	[sPanel setTitle:@"Choose a filename"];
	
	// Display the dialog.  If the OK button was pressed,
	// process the files.
	if ( [sPanel runModal] == NSOKButton )
	{
		NSString* fileName = [[sPanel URL] path];
		
		const char *file = [fileName fileSystemRepresentation];
		NSLog(@"Save File: %s", file);
				
		ZNT fd((char *)file, *tg_final);
		char msg;
		msg = fd.SaveFile();

		if (msg == -1)
			NSLog(@"Err: saveZNTfile --> Could not save the file.");
	}
}


// Save Final Texture to TGA
- (IBAction) saveTGAfile:(id)sender
{
    NSArray *fileTypes = [NSArray arrayWithObjects:@"tga", nil];

	// Create the File Save Panel class.
	NSSavePanel* sPanel = [NSSavePanel savePanel];
	//[oPanel setParentWindow:mainWindow];	// Define the parent of our dialog
	[sPanel setFloatingPanel:NO];				// When we move our parent window, the dialog moves with it
	[sPanel setAllowedFileTypes:fileTypes];
	[sPanel setCanCreateDirectories:YES];		// Enable the creation of directories in the dialog
	[sPanel setAlphaValue:0.95];				// Alpha value
	[sPanel setTitle:@"Choose a filename"];
	
	// Display the dialog.  If the OK button was pressed,
	// process the files.
	if ( [sPanel runModal] == NSOKButton )
	{
		NSString* fileName = [[sPanel URL] path];
		
		const char *file = [fileName fileSystemRepresentation];
		NSLog(@"Save File: %s", file);
		
		TGA tga(file);
		char msg;
		msg = tga.Save(tg_final->t.w, tg_final->t.h, tg_final->t.data);
		
		if (msg == -1)
			NSLog(@"Err: saveTGAfile --> Could not save the file.");
	}
}

#pragma mark Render Texture
// Regenerate and render final texture
- (void) renderFinal:(id)sender
{
	CTextGen *tmptex;
	
	tmptex = tg_final;
	[self UpdateLayerList:nil];
	tmptex->Regenerate();
	NSLog(@"Regenerating Final texture... done!!");
	[self LogFinalTexInfo];
	
	// Convert buffer to NSBitmapImageRep
	NSBitmapImageRep* bmp = [[NSBitmapImageRep alloc]	initWithBitmapDataPlanes:(unsigned char **)&tmptex->t.data
																	pixelsWide: tmptex->t.w
																	pixelsHigh: tmptex->t.h
																 bitsPerSample: 8
															   samplesPerPixel: tmptex->t.iformat
																	  hasAlpha: NO
																	  isPlanar: NO
																colorSpaceName: NSCalibratedRGBColorSpace
																   bytesPerRow: (tmptex->t.w*tmptex->t.iformat)
																  bitsPerPixel: 24];
	
	// Store the NSBitmapImageRep in a NSImage structure
	NSImage *img = [[NSImage alloc] initWithSize:[bmp size] ];
	[img addRepresentation: bmp];
	[bmp release];
	
	if (img)
	{
		[IVFinaltexture setImage:img];
		[img release];
	}
}

// Regenerate and render temp texture
- (void) renderTemp:(id)sender
{
	CTextGen *tmptex;
	
	for (int i=0; i<TMP_TEXTURES; i++)
	{
		tmptex = tg_temptext[i];
		tmptex->Regenerate();
		NSLog(@"Regenerating Temporary text #%d... done!! [Size: %d]",i, tmptex->dtexsize);
		[self LogTmpTexInfo];
	}
	
	tmptex = tg_temptext[0];
	// Convert buffer to NSBitmapImageRep
	NSBitmapImageRep* bmp = [[NSBitmapImageRep alloc]	initWithBitmapDataPlanes:(unsigned char **)&tmptex->t.data
																	pixelsWide: tmptex->t.w
																	pixelsHigh: tmptex->t.h
																 bitsPerSample: 8
															   samplesPerPixel: tmptex->t.iformat
																	  hasAlpha: NO
																	  isPlanar: NO
																colorSpaceName: NSCalibratedRGBColorSpace
																   bytesPerRow: (tmptex->t.w*tmptex->t.iformat)
																  bitsPerPixel: 24];
	
	// Store the NSBitmapImageRep in a NSImage structure
	NSImage *img = [[NSImage alloc] initWithSize:[bmp size] ];
	[img addRepresentation: bmp];
	[bmp release];
	
	if (img)
	{
		[IVTemptexture setImage:img];
		[img release];
	}
}

#pragma mark Layer Management

// Add the temporary texture to the final texture (at the end)
-(void)AddLayer:(id)sender
{
	tg_final->dtex.insert(tg_final->dtex.end(), tg_temptext[0]->dtex[0]);
	tg_final->dtexsize = tg_final->dtex.size();
	int i = tg_final->dtex.size()-1; // ID de la textura que acabem d'afegir
	tg_final->dtex[i].operation		=	[CBOperation indexOfSelectedItem];
	
	[self renderFinal:nil];
}

// Add the effect to the final texture (at the end)
-(void)AddEffect:(int)type
{
	if (type<100)
	{
		NSLog(@"WARNING! Effect not recognized!");
		return;
	}
	if (tg_final->dtex.size()>0)
	{
		COneTextGen tg;
		tg.type = type;
		tg_final->dtex.insert(tg_final->dtex.end(), tg);
		tg_final->dtexsize = tg_final->dtex.size();
		int i = tg_final->dtex.size()-1; // ID de la textura que acabem d'afegir
		tg_final->dtex[i].operation	= 0;
		
		[self renderFinal:nil];
	}
	else
	{
		NSAlert * askToContinue = [NSAlert alertWithMessageText:@"Impossible to perform"
												  defaultButton:nil
												alternateButton:nil
													otherButton:nil
									  informativeTextWithFormat:@"You cannot add effects without a base texture."];
		[askToContinue runModal];
		NSLog(@"AddEffect: You cannot add effects without a base texture");
		return;
	}
}

- (IBAction) AddEffect_bw:(id)sender		{[self AddEffect:100];}
- (IBAction) AddEffect_r2polar:(id)sender	{[self AddEffect:101];}
- (IBAction) AddEffect_blur:(id)sender		{[self AddEffect:102];}
- (IBAction) AddEffect_mblur:(id)sender		{[self AddEffect:103];}
- (IBAction) AddEffect_edges1:(id)sender	{[self AddEffect:104];}
- (IBAction) AddEffect_edges2:(id)sender	{[self AddEffect:105];}
- (IBAction) AddEffect_sharpen1:(id)sender	{[self AddEffect:106];}
- (IBAction) AddEffect_sharpen2:(id)sender	{[self AddEffect:107];}
- (IBAction) AddEffect_sharpen3:(id)sender	{[self AddEffect:108];}
- (IBAction) AddEffect_emboss1:(id)sender	{[self AddEffect:109];}
- (IBAction) AddEffect_emboss2:(id)sender	{[self AddEffect:110];}
- (IBAction) AddEffect_mean1:(id)sender		{[self AddEffect:111];}
- (IBAction) AddEffect_mean2:(id)sender		{[self AddEffect:112];}
- (IBAction) AddEffect_custom3x3:(id)sender
{
	// TODO
	[self AddEffect:150];
	int size = tg_final->dtex.size()-1;
	if (size>0)
	{
		if (tg_final->dtex[size].type==150)
			[self showEfxCustom:nil];
	}
	NSLog(@"TODO: Add effect custom 3x3");
}
- (IBAction) AddEffect_colorize:(id)sender
{
    // TODO
	[self AddEffect:151];
	int size = tg_final->dtex.size()-1;
	if (size>0)
	{
		if (tg_final->dtex[size].type==151)
			[self showEfxColorize:nil];
	}
	NSLog(@"TODO: Add effect colorize");
}
- (IBAction) AddEffect_gradblur:(id)sender
{
    // TODO
	[self AddEffect:152];
	int size = tg_final->dtex.size()-1;
	if (size>0)
	{
		if (tg_final->dtex[size].type==152)
			[self showEfxGradBlur:nil];
	}
	NSLog(@"TODO: Add effect gradient blur");
}


// Delete the selected layer from the final texture
-(void)DeleteLayer:(id)sender
{
	int s = [layerlist selectedRow];
	if (s>=0)
	{
		NSLog (@"Deleting Layer number %i",s);
		tg_final->dtex[s].Init();
		tg_final->dtex.erase(tg_final->dtex.begin()+s);
		tg_final->dtexsize = tg_final->dtex.size();
		[self renderFinal:nil];
	}
}

// Delete all layers from the final texture
-(void)DeleteAllLayers:(id)sender
{
	NSAlert * askToContinue = [NSAlert alertWithMessageText:@"Are you sure that you want to delete all layers?"
											  defaultButton:@"Yes"
											alternateButton:@"No"
												otherButton:nil
								  informativeTextWithFormat:@" The Final Texture (your work of hours and hours!) will be lost!!!"];
	if( [askToContinue runModal] == NSAlertDefaultReturn ) {
		tg_final->Init();
		[self renderFinal:nil];
	}
	
}

- (void) UpdateOperationFromLayer:(int)layer_num AndOperation:(int)operation
{
	tg_final->dtex[layer_num].operation = operation;
	[self renderFinal:nil];
}

// Update the layer list
-(void)UpdateLayerList:(id)sender
{
	int selected = [layerlist selectedRow];

	if (selected<0)
		selected = 0;
	if (selected>=[layerlist numberOfRows])
		selected = 0;
	[layerlist deleteallLayers];
	int i;
	TEXTURE *temp_t;
	temp_t = &(tg_final->t);
	
	for (i=0; i<tg_final->dtex.size(); i++)	{
		NSImage* sourceImage;
		COneTextGen *myTex = &tg_final->dtex[i];
		if (myTex->type<100)
			myTex->Regenerate(*temp_t, temp_t->data);	// Regenerate each texture
		else
			myTex->ApplyEffect(*temp_t, temp_t->data);	// Apply each effect
		
		NSBitmapImageRep* imageRep;
		imageRep=[[[NSBitmapImageRep alloc] initWithBitmapDataPlanes:(unsigned char **)&temp_t->data
														  pixelsWide:temp_t->w
														  pixelsHigh:temp_t->h
													   bitsPerSample:8
													 samplesPerPixel:temp_t->iformat
															hasAlpha:NO
															isPlanar:NO
													  colorSpaceName:NSCalibratedRGBColorSpace
														 bytesPerRow:(temp_t->w*temp_t->iformat)
														bitsPerPixel:(temp_t->iformat*8)] autorelease];
		
		
		sourceImage = [[[NSImage alloc] initWithSize:NSMakeSize(temp_t->w, temp_t->h)] autorelease];
		[sourceImage addRepresentation:imageRep];
		if ([sourceImage isValid])
		{
			NSImage* thumbnail = [sourceImage imageByScalingProportionallyToSize:NSMakeSize(32,32)];
			NSString* props = [NSString stringWithFormat:@"Type: %d",myTex->type];
			[layerlist addLayer:@"TRUE" image:thumbnail operation:myTex->operation properties:props];
		}
	}
	
	if (selected <= tg_final->dtex.size())
		[layerlist selectRow:selected];
}

-(void)LayerUp:(id)sender
{
	int selected = [layerlist selectedRow];
	if ((selected > 0) && (selected < [layerlist numberOfRows]))
	{
		COneTextGen temp = tg_final->dtex[selected-1];
		tg_final->dtex[selected-1] = tg_final->dtex[selected];
		tg_final->dtex[selected] = temp;
		[layerlist selectRow:(selected-1)];
		[self renderFinal:nil];
	}
}

-(void)LayerDown:(id)sender
{	
	int selected = [layerlist selectedRow];
	if ((selected >= 0) && (selected < ([layerlist numberOfRows]-1)))
	{
		COneTextGen temp = tg_final->dtex[selected+1];
		tg_final->dtex[selected+1] = tg_final->dtex[selected];
		tg_final->dtex[selected] = temp;
		[layerlist selectRow:(selected+1)];
		[self renderFinal:nil];
	}
}

#pragma mark Panels Management

- (void) hideAllPanels
{
	//	[TGCelularPanel orderOut:nil];
	[TGPlainCtrl hideCtrl];
	[TGCelularCtrl hideCtrl];
}


- (IBAction) showPlain:(id)sender
{
	if ([TGPlainCtrl isvisibleCtrl])
		[TGPlainCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGPlainCtrl showCtrl];
		[TGPlainCtrl redraw:nil];
	}
}

- (void) GetPlainData:(T_PLAIN)t_data
{
	tg_temptext[0]->dtex[0].type=0;
	tg_temptext[0]->dtex[0].plain = t_data;
}

- (IBAction) showNoise:(id)sender
{
	if ([TGNoiseCtrl isvisibleCtrl])
		[TGNoiseCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGNoiseCtrl showCtrl];
		[TGNoiseCtrl redraw:nil];
	}
}

- (void) GetNoiseData:(T_NOISE)t_data
{
	tg_temptext[0]->dtex[0].type=1;
	tg_temptext[0]->dtex[0].noise = t_data;
}


- (IBAction) showCelular:(id)sender
{
	if ([TGCelularCtrl isvisibleCtrl])
		[TGCelularCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGCelularCtrl showCtrl];
		[TGCelularCtrl redraw:nil];
	}
}

- (IBAction) showPerlin:(id)sender
{
	if ([TGPerlinCtrl isvisibleCtrl])
		[TGPerlinCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGPerlinCtrl showCtrl];
		[TGPerlinCtrl redraw:nil];
	}
}

- (void)GetPerlinData:(T_PERLIN)t_data
{
	tg_temptext[0]->dtex[0].type=2;
	tg_temptext[0]->dtex[0].perlin= t_data;
}

- (void)GetCelularData:(T_CELULAR)t_data
{
	tg_temptext[0]->dtex[0].type=3;
	tg_temptext[0]->dtex[0].celular = t_data;
}


- (IBAction) showPlasma:(id)sender
{
	if ([TGPlasmaCtrl isvisibleCtrl])
		[TGPlasmaCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGPlasmaCtrl showCtrl];
		[TGPlasmaCtrl redraw:nil];
	}
}

- (void)GetPlasmaData:(T_PLASMA)t_data
{
	tg_temptext[0]->dtex[0].type=4;
	tg_temptext[0]->dtex[0].plasma= t_data;
}


- (IBAction) showBlob:(id)sender
{
	if ([TGBlobCtrl isvisibleCtrl])
		[TGBlobCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGBlobCtrl showCtrl];
		[TGBlobCtrl redraw:nil];
	}
}

- (void)GetBlobData:(T_BLOB)t_data
{
	tg_temptext[0]->dtex[0].type=5;
	tg_temptext[0]->dtex[0].blob = t_data;
}



- (IBAction) showEfxCustom:(id)sender
{
	if ([TGEfxCustomCtrl isvisibleCtrl])
		[TGEfxCustomCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGEfxCustomCtrl showCtrl];
		[TGEfxCustomCtrl redraw:nil];
	}
}

- (void)GetEfxCustom:(T_EFF_CUSTOM)t_data
{
//	tg_temptext[0]->dtex[0].type=150;
//	tg_temptext[0]->dtex[0].eff_cust = t_data;
	int size = tg_final->dtex.size()-1;
	if (size>0)
	{
		if (tg_final->dtex[size].type == 150)
			tg_final->dtex[size].eff_cust = t_data;
		[self renderFinal:nil];
	}
//		tg_temptext[0]->dtex[0].type=150;
	//	tg_temptext[0]->dtex[0].eff_cust = t_data;
}

- (IBAction) showEfxColorize:(id)sender
{
	if ([TGEfxColorizeCtrl isvisibleCtrl])
		[TGEfxColorizeCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGEfxColorizeCtrl showCtrl];
		[TGEfxColorizeCtrl redraw:nil];
	}
}

- (void)GetEfxColorize:(T_EFF_COLOR)t_data
{
	int size = tg_final->dtex.size()-1;
	if (size>0)
	{
		if (tg_final->dtex[size].type == 151)
			tg_final->dtex[size].eff_color = t_data;
		[self renderFinal:nil];
	}
}

- (IBAction) showEfxGradBlur:(id)sender
{
	if ([TGEfxGradBlurCtrl isvisibleCtrl])
		[TGEfxGradBlurCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGEfxGradBlurCtrl showCtrl];
		[TGEfxGradBlurCtrl redraw:nil];
	}
}

- (void)GetEfxGradBlur:(T_EFF_GRADBLUR)t_data
{
	int size = tg_final->dtex.size()-1;
	if (size>0)
	{
		if (tg_final->dtex[size].type == 152)
			tg_final->dtex[size].eff_gradblur = t_data;
		[self renderFinal:nil];
	}
}

#pragma mark Logs
- (void) LogFinalTexInfo
{
	int i=0;
	CTextGen *tmptex = tg_final;
	NSLog(@" :: INFO Final text ::");
	if (tmptex->dtex.size()!=tmptex->dtexsize)
		NSLog(@"    ERROR: sizes are different! (dtex and dtexsize)");
	NSLog(@"    Size dtexsize: %d", tmptex->dtexsize);
	for (i=0;i<tmptex->dtexsize;i++)
	{
		NSLog(@"    Layer %d, type: %d", i, tmptex->dtex[i].type);
	}
	
}

- (void) LogTmpTexInfo
{
	for (int i=0; i<TMP_TEXTURES; i++)
	{
		CTextGen *tmptex = tg_temptext[i];
		NSLog(@" :: INFO Temporary text #%d [Size: %d]",i, tmptex->dtexsize);
		NSLog(@"    type: %d", tmptex->dtex[0].type);
	}
}


@end




#pragma mark -
#pragma mark Accessors


/////////////////////////////////////////////////////////////////
/// NSImage EXTRAS
/////////////////////////////////////////////////////////////////


@implementation NSImage (Extras)

- (NSImage*)imageByScalingProportionallyToSize:(NSSize)targetSize
{
	NSImage* sourceImage = self;
	NSImage* newImage = nil;
	
	NSAutoreleasePool* poool = [[NSAutoreleasePool alloc] init];
	
	if ([sourceImage isValid])
	{
		NSSize imageSize = [sourceImage size];
		float width = imageSize.width;
		float height = imageSize.height;
		
		float targetWidth = targetSize.width;
		float targetHeight = targetSize.height;
		
		// scaleFactor will be the fraction that we'll
		// use to adjust the size. For example, if we shrink
		// an image by half, scaleFactor will be 0.5. the
		// scaledWidth and scaledHeight will be the original,
		// multiplied by the scaleFactor.
		//
		// IMPORTANT: the "targetHeight" is the size of the space
		// we're drawing into. The "scaledHeight" is the height that
		// the image actually is drawn at, once we take into
		// account the ideal of maintaining proportions
		
		float scaleFactor = 0.0;
		float scaledWidth = targetWidth;
		float scaledHeight = targetHeight;
		
		NSPoint thumbnailPoint = NSMakePoint(0,0);
		
		// since not all images are square, we want to scale
		// proportionately. To do this, we find the longest
		// edge and use that as a guide.
		
		if ( NSEqualSizes( imageSize, targetSize ) == NO )
		{
			// use the longeset edge as a guide. if the
			// image is wider than tall, we'll figure out
			// the scale factor by dividing it by the
			// intended width. Otherwise, we'll use the
			// height.
			
			float widthFactor = targetWidth / width;
			float heightFactor = targetHeight / height;
			
			if ( widthFactor < heightFactor )
				scaleFactor = widthFactor;
			else
				scaleFactor = heightFactor;
			
			// ex: 500 * 0.5 = 250 (newWidth)
			
			scaledWidth = width * scaleFactor;
			scaledHeight = height * scaleFactor;
			
			// center the thumbnail in the frame. if
			// wider than tall, we need to adjust the
			// vertical drawing point (y axis)
			
			if ( widthFactor < heightFactor )
				thumbnailPoint.y = (targetHeight - scaledHeight) * 0.5;
			
			else if ( widthFactor > heightFactor )
				thumbnailPoint.x = (targetWidth - scaledWidth) * 0.5;
		}
		
		
		// create a new image to draw into
		newImage = [[NSImage alloc] initWithSize:targetSize];
		
		// once focus is locked, all drawing goes into this NSImage instance
		// directly, not to the screen. It also receives its own graphics
		// context.
		//
		// Also, keep in mind that we're doing this in a background thread.
		// You only want to draw to the screen in the main thread, but
		// drawing to an offscreen image is (apparently) okay.
		
		[newImage lockFocus];
		
		NSRect thumbnailRect;
		thumbnailRect.origin = thumbnailPoint;
		thumbnailRect.size.width = scaledWidth;
		thumbnailRect.size.height = scaledHeight;
		
		[sourceImage drawInRect: thumbnailRect
					   fromRect: NSZeroRect
					  operation: NSCompositeSourceOver
					   fraction: 1.0];
		
		[newImage unlockFocus];
		
	}
	[poool release];
	
	return [newImage autorelease];
}

@end
