#import "c_structs.h"
#import "../textgenlib/main.h"

@interface TGPlasmaControl : NSObject {
    IBOutlet NSColorWell *TGPlColor1;
    IBOutlet NSColorWell *TGPlColor2;
	IBOutlet NSSlider *TGPlposX, *TGPlposY;
	IBOutlet NSSlider *TGPltam1, *TGPltam2, *TGPltam3, *TGPltam4;
	IBOutlet NSSlider *TGPlseed;
	IBOutlet NSSlider *TGPlzoom;
    IBOutlet NSPanel *TGPlasmaPanel;
    IBOutlet NSObject *_parent;
@public
	c_gfx	*gfx;
	T_PLASMA t_data; // Texture data
}

- (void) showCtrl;
- (void) hideCtrl;
- (BOOL) isvisibleCtrl;

- (IBAction) redraw:(id)sender;
- (NSColor *) getColor1;
- (NSColor *) getColor2;
- (void) getPosX;
//- (void) getPosY;

@end
