//
//  TGGUIControl.m
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "TGGUIControl.h"


#include "znt.h"

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
		NSLog(@"Re-initializing temporary text #%d [Size: %d]... done!!",i, tmptex->dtex.size());
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
	
	// Display the dialog.  If the OK button was pressed,
	// process the files.
	if ( [oPanel runModalForDirectory:nil file:nil types:fileTypes] == NSOKButton )
	{
		// Get an array containing the full filenames of all
		// files and directories selected.
		NSArray* files = [oPanel filenames];
		
		// Loop through all the files and process them.
		for( i = 0; i < [files count]; i++ )
		{
			NSString* fileName = [files objectAtIndex:i];
			NSLog(fileName);
			const char *temp = [fileName fileSystemRepresentation];
			int len = strlen(temp);
			char sysctlPath [len+1];
			strcpy(sysctlPath, temp);
			
			ZNT fd(sysctlPath, *tg_final);
			char msg;
			msg = fd.LoadFile();
			
			if (msg == -1)
				NSLog(@"Err: openZNTfile --> Could not read the file. File version is not valid.");
			if (msg == -2)
				NSLog(@"Err: openZNTfile --> Could not read the file. File is not valid.");
			
			[self renderFinal:nil];			
		}
	}
}

// Save Final Texture to TGA
- (void) SaveToTGA:(id)sender
{
	NSLog(@"TODO: SaveToTGA");
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
	COneTextGen tg;
	tg.type = type;
	tg_final->dtex.insert(tg_final->dtex.end(), tg);
	tg_final->dtexsize = tg_final->dtex.size();
	int i = tg_final->dtex.size()-1; // ID de la textura que acabem d'afegir
	tg_final->dtex[i].operation	= 0;
	
	[self renderFinal:nil];
}

- (void) AddEffect_bw:(id)sender		{[self AddEffect:100];}
- (void) AddEffect_r2polar:(id)sender	{[self AddEffect:101];}
- (void) AddEffect_blur:(id)sender		{[self AddEffect:102];}
- (void) AddEffect_mblur:(id)sender		{[self AddEffect:103];}
- (void) AddEffect_edges1:(id)sender	{[self AddEffect:104];}
- (void) AddEffect_edges2:(id)sender	{[self AddEffect:105];}
- (void) AddEffect_sharpen1:(id)sender	{[self AddEffect:106];}
- (void) AddEffect_sharpen2:(id)sender	{[self AddEffect:107];}
- (void) AddEffect_sharpen3:(id)sender	{[self AddEffect:108];}
- (void) AddEffect_emboss1:(id)sender	{[self AddEffect:109];}
- (void) AddEffect_emboss2:(id)sender	{[self AddEffect:110];}
- (void) AddEffect_mean1:(id)sender		{[self AddEffect:111];}
- (void) AddEffect_mean2:(id)sender		{[self AddEffect:112];}
- (void) AddEffect_custom3x3:(id)sender
{
	// TODO
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

- (void)GetPlainData:(T_PLAIN)t_data
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

- (void)GetNoiseData:(T_NOISE)t_data
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

- (void)GetCelularData:(T_CELULAR)t_data
{
	tg_temptext[0]->dtex[0].type=3;
	tg_temptext[0]->dtex[0].celular = t_data;
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
