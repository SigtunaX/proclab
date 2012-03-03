#import "TGPlasmaControl.h"
#import "../TGGUIControl.h"

@implementation TGPlasmaControl

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
    
    [TGPlColor1 setColor:[col1 colorUsingColorSpaceName:NSCalibratedRGBColorSpace]];
    [TGPlColor2 setColor:[col2 colorUsingColorSpaceName:NSCalibratedRGBColorSpace]];
}

- (BOOL) isvisibleCtrl
{
	return [TGPlasmaPanel isVisible];
}

- (void) showCtrl
{
	[TGPlasmaPanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGPlasmaPanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
	[self getColor1];								// Get Color data
	[self getColor2];								// Get Color data
	[self getPosX];									// Get PosX
///	[self getPosY];									// Get PosY
	[(TGGUIControl*)_parent GetPlasmaData:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}

- (NSColor*) getColor1
{
	NSColor *myColor = [TGPlColor1 color];
	t_data.c.R = 255*[myColor redComponent];
	t_data.c.G = 255*[myColor greenComponent];
	t_data.c.B = 255*[myColor blueComponent];
	return myColor;
}

- (NSColor*) getColor2
{
	NSColor *myColor = [TGPlColor2 color];
	t_data.s.R = 255*[myColor redComponent];
	t_data.s.G = 255*[myColor greenComponent];
	t_data.s.B = 255*[myColor blueComponent];
	return myColor;
}

- (void) getPosX
{
	t_data.posX = [TGPlposX intValue];
	t_data.posY = [TGPlposY intValue];
	t_data.tam1 = [TGPltam1 intValue];
	t_data.tam2 = [TGPltam2 intValue];
	t_data.tam3 = [TGPltam3 intValue];
	t_data.tam4 = [TGPltam4 intValue];
	t_data.seed = [TGPlseed intValue];
	t_data.zoom = [TGPlzoom intValue];
}

@end
