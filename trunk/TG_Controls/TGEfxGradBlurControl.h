#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGEfxGradBlurControl : NSObject {
	IBOutlet NSSlider *TGPSteps;
	IBOutlet NSSlider *TGPStepX;
	IBOutlet NSSlider *TGPStepY;
    IBOutlet NSPanel *TGEfxGradBlurPanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_EFF_GRADBLUR t_data; // Effect data
}

- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (void) getValues;

@end
