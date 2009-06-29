/* MainGUIControl */

#import <Cocoa/Cocoa.h>


@interface MainGUIControl : NSObject
{
	IBOutlet NSWindow *mainWindow;
	IBOutlet NSView *ViewContainer;
	IBOutlet NSView *ViewTextureGen;
	IBOutlet NSView *ViewTextureLib;
}

- (IBAction)showTextureGen:(id)sender;
- (IBAction)showTextureLib:(id)sender;
- (void) didAddSubview:(NSView *) view;
@end
