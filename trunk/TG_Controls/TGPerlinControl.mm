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

	NSColor *myColorS = [TGPColorS color];
	t_data.s.R = 255*[myColorS redComponent];
	t_data.s.G = 255*[myColorS greenComponent];
	t_data.s.B = 255*[myColorS blueComponent];	
}

@end
