//
//  TGGUIControl.m
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

#import "TGGUIControl.h"

#import "GLImage.h"
#include "znt.h"

@implementation TGGUIControl

- (id)init
{
	if (self = [super init])
	{
		NSLog (@"====== ProcLab :: Loading ======");
		[self setLayerList:[NSArray array]];
		[self init_prepareText];
	}
	return self;
}

- (void)dealloc
{
	[self setLayerList:nil];
	[super dealloc];
}

- (void)awakeFromNib
{
}


// Add a texture to the texture stack
- (void)AddText:(id)sender
{
	NSString *msg = [@"TODO: Add texture with operation: " stringByAppendingString:[CBOperation stringValue]];
	NSLog([msg stringByAppendingString:@"Add texture number: "]);
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


// Save Final Texture to TGA
- (void) SaveToTGA:(id)sender
{
	NSLog(@"TODO: SaveToTGA");
}

- (void) hideAllPanels
{
	[TGCelularPanel orderOut:nil];
	[TGPlainCtrl hideCtrl];
}

- (IBAction) showCelular:(id)sender
{
}

- (IBAction) showPlain:(id)sender
{
	
	if ([TGPlainCtrl isvisibleCtrl])
		[TGPlainCtrl hideCtrl];
	else
	{
		[self hideAllPanels];
		[TGPlainCtrl showCtrl];
		tg_temptext[0]->dtex[0].type=0;
		[TGPlainCtrl redraw:nil];
	}
}

- (void)GetPlainData:(T_PLAIN)t_data
{
	tg_temptext[0]->dtex[0].type=0;
	tg_temptext[0]->dtex[0].plain = t_data;
}


// Update the layer list
-(void)UpdateLayerList:(id)sender
{
	int i=0;
	// list of layers
    NSMutableArray* mylayerList = [self layerList];
	
	// Borrem tots els elements de la llista
	[mylayerList removeAllObjects];
	
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
			
			GLImage* spImage = [[GLImage alloc] init];
			[spImage setTitle:[NSString stringWithFormat:@"TEST layer\nOperation: ADD %d", random()]];
			[spImage setDefaultThumbnail:thumbnail];
			//button
			/* NSButton *btn = [[NSButton alloc] init];
						[btn setButtonType:NSOnOffButton];
						[btn setState:NSOnState];
						[spImage setVisible:btn]; 
			 */
			
			NSComboBoxCell* comboCell = [[NSComboBoxCell alloc] init];
			[spImage setOperation:comboCell];
			[comboCell setControlSize:NSSmallControlSize];
			[comboCell setFont:[NSFont systemFontOfSize:[NSFont smallSystemFontSize]]];
			[comboCell setEditable:NO];
			[comboCell addItemWithObjectValue:[NSString stringWithFormat:@"pepe1 %i",i]];
			[comboCell addItemWithObjectValue:@"pepe2"];
			[comboCell addItemWithObjectValue:@"pepe3"];
			[comboCell addItemWithObjectValue:@"pepe4"];
			[comboCell setAction:@selector(selectedItem:)];
			[comboCell setTarget:self];

			//[comboCell selec];
			
			/*
			 NSComboBoxCell *comboCell;
			 NSTableColumn *combocolumn = [[ofsbliste tableColumns]
			 objectAtIndex:0];
			 
			 comboCell = [[NSComboBoxCell alloc] init];
			 [combocolumn setDataCell:comboCell];
			 [comboCell setControlSize:NSSmallControlSize];
			 [comboCell setFont:[NSFont systemFontOfSize:[NSFont
			 smallSystemFontSize]]];
			 [comboCell setEditable:YES];
			 [comboCell setCompletes:YES];
			 [comboCell setAction:@selector(selectedItem:)];
			 [comboCell setTarget:self];
			 */
			
			
			[mylayerList addObject:spImage];
			[spImage release];
		}
	}
	[self performSelectorOnMainThread: @selector(setLayerList:)
						   withObject: mylayerList
						waitUntilDone: YES];
}

// Add the temporary texture to the final texture (at the end)
-(void)AddLayer:(id)sender
{
	tg_final->dtex.insert(tg_final->dtex.end(), tg_temptext[0]->dtex[0]);
	int i = tg_final->dtex.size()-1; // ID de la textura que acabem d'afegir
	tg_final->dtex[i].operation		=	0; // TODO : HARDCODE
	[self UpdateLayerList:nil];
}

// Delete the selected layer from the final texture
-(void)DeleteLayer:(id)sender
{
	int s = [TVLayerList selectedRow];
	if (s>=0)
	{
		NSLog (@"Deleting Layer number %i",s);
		tg_final->dtex[s].Init();	// Alliberem memòria
		tg_final->dtex.erase(tg_final->dtex.begin()+s);
		[self UpdateLayerList:nil];
	}
}

// Delete all layers from the final texture
-(void)DeleteAllLayers:(id)sender
{
	tg_final->Init();
	[self UpdateLayerList:nil];
}


#pragma mark -
#pragma mark Utilities

// Prepare texture data
-(void)init_prepareText
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

	
	/*//////// TEST - Fill the layer list
	
    // the list of images we'll loaded from this directory
    NSMutableArray* layerList = [self layerList];
	
	for (i=0; i<10; i++)
	{
		
		// try to load the file as an NSImage, and continue only if it's valid
		NSImage* sourceImage = [[NSImage alloc] initWithContentsOfFile:@"/Users/xphere/Pictures/avatar.gif"];
		//NSButton* visible = [[NSButton alloc] init];
		if ([sourceImage isValid])
		{
			// drawing the entire, full-sized image every time the table view
			// scrolls is way too slow, so instead will draw a thumbnail version
			// into a separate NSImage, which acts as a cache
			
			//NSImage* thumbnail = [sourceImage imageByScalingProportionallyToSize:NSMakeSize(32,32)];
			NSImage* thumbnail = sourceImage;
			//[visible setState: NSOnState ];
			
			
			// create a new GLImage
			GLImage* spImage = [[GLImage alloc] init];
			
			// set the path of the on-disk image and our cache instance
			[spImage setTitle:[NSString stringWithFormat:@"TEST layer: %d\nOperation: ADD", i]];
			[spImage setDefaultThumbnail:thumbnail];
			//[spImage setVisible:visible]; //TODO: Fix d'aixo... poder afegir un botó per cadascun.
			
			// add to the SPImage array
			[layerList addObject:spImage];
			
			// adding an object to an array retains it, so we
			// can release our reference
			[spImage release];
		}
		//[visible release];
		// now release the image we created.
		[sourceImage release]; 
	}
	// we want to actually set the new value in the main thread, to
	// avoid any mix-ups with Cocoa Bindings
	[self performSelectorOnMainThread: @selector(setLayerList:)
						   withObject: layerList
						waitUntilDone: YES];
	
	[layerList release];

	///////////////
	*/
	
}

#pragma mark -
#pragma mark Accessors

- (NSMutableArray*)layerList
{
	return _layerList;
}

- (void)setLayerList:(NSArray*)aValue
{
	NSMutableArray* oldLayerList = _layerList;
	_layerList = [aValue mutableCopy];
	[oldLayerList release];
//    [self setImportingImages:NO];
}



@end






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
