#import "TGEfxColorizeControl.h"
#import "../TGGUIControl.h"

@implementation TGEfxColorizeControl

- (id)init
{
	if (self = [super init])
	{
	}
	return self;
}

- (void)dealloc
{
	[super dealloc];
}

- (void)awakeFromNib
{
    NSColor *col1 = [NSColor grayColor];
    NSColor *col2 = [NSColor orangeColor];
    
    [TGPColor1 setColor:[col1 colorUsingColorSpaceName:NSCalibratedRGBColorSpace]];
    [TGPColor2 setColor:[col2 colorUsingColorSpaceName:NSCalibratedRGBColorSpace]];

}

- (BOOL) isvisibleCtrl
{
	return [TGEfxColorizePanel isVisible];
}

- (void) showCtrl
{
	[TGEfxColorizePanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGEfxColorizePanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
    [self getValues];
		
	[(TGGUIControl*)_parent GetEfxColorize:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}


- (void) getValues
{
    
	NSColor *myColorC = [TGPColor1 color];
	NSColor *myColorS = [TGPColor2 color];

	t_data.c.R = 255*[myColorC redComponent];
	t_data.c.G = 255*[myColorC greenComponent];
	t_data.c.B = 255*[myColorC blueComponent];
    
    t_data.s.R = 255*[myColorS redComponent];
	t_data.s.G = 255*[myColorS greenComponent];
	t_data.s.B = 255*[myColorS blueComponent];
}


@end
