#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGEfxColorizeControl : NSObject {
    IBOutlet NSColorWell *TGPColor1;
    IBOutlet NSColorWell *TGPColor2;
    IBOutlet NSPanel *TGEfxColorizePanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_EFF_COLOR t_data; // Effect data
}

- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (void) getValues;

@end
