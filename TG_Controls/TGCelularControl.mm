#import "TGCelularControl.h"
#import "../TGGUIControl.h"

@implementation TGCelularControl

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
	[TGCType selectItemAtIndex:0];
}

- (BOOL) isvisibleCtrl
{
	return [TGCelularPanel isVisible];
}

- (void) showCtrl
{
	[TGCelularPanel makeKeyAndOrderFront:nil];
}

- (void) hideCtrl
{
	[TGCelularPanel orderOut:nil];
}

- (IBAction)redraw:(id)sender
{
	[self getColor1];								// Get Color data
	[self getColor2];								// Get Color data
	[self getType];									// Get Type
	[(TGGUIControl*)_parent GetCelularData:t_data];	// Send the data to the parent
	[(TGGUIControl*)_parent renderTemp:nil];		// Redraw
}

- (NSColor*) getColor1
{
	NSColor *myColor = [TGCColor1 color];
	t_data.c.R = 255*[myColor redComponent];
	t_data.c.G = 255*[myColor greenComponent];
	t_data.c.B = 255*[myColor blueComponent];
	return myColor;
}

- (NSColor*) getColor2
{
	NSColor *myColor = [TGCColor2 color];
	t_data.s.R = 255*[myColor redComponent];
	t_data.s.G = 255*[myColor greenComponent];
	t_data.s.B = 255*[myColor blueComponent];
	return myColor;
}

- (void) getType
{
	t_data.type = [TGCType indexOfSelectedItem];
}

@end
