#import "TGPerlinControl.h"
#import "../TGGUIControl.h"

@implementation TGPerlinControl

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
}

- (BOOL) isvisibleCtrl
{
	return [TGPerlinPanel isVisible];
}

- (void) showCtrl
{
	[TGPerlinPanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGPerlinPanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
	[self getValues];
	[(TGGUIControl*)_parent GetPerlinData:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}

- (void) getValues
{

	NSColor *myColorC = [TGPColorC color];
	t_data.c.R = 255*[myColorC redComponent];
	t_data.c.G = 255*[myColorC greenComponent];
	t_data.c.B = 255*[myColorC blueComponent];

	t_data.zoom = (unsigned char)[TGPZoom floatValue];
	t_data.persistence = (unsigned char)[TGPPersistence floatValue];
	t_data.octaves = (unsigned char)[TGPOctaves floatValue];
}

@end
