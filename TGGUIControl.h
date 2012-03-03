//
//  TGGUIControl.h
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

//#import <UIKit/UIKit.h
#import "TG_Controls/TGPlainControl.h"
#import "TG_Controls/TGNoiseControl.h"
#import "TG_Controls/TGBlobControl.h"
#import "TG_Controls/TGCelularControl.h"
#import "TG_Controls/TGPlasmaControl.h"
#import "TG_Controls/TGPerlinControl.h"
#import "TG_Controls/TGEfxCustomControl.h"
#import "TG_Controls/TGEfxColorizeControl.h"
#import "TG_Controls/TGEfxGradBlurControl.h"

#import "c_structs.h"
#import "textgenlib/main.h"
#import "controls/tableview.h"

#define TMP_TEXTURES 1

@interface TGGUIControl : NSObject {
	/* Controles de la ventana principal */
	IBOutlet NSView *mainTGView;
	IBOutlet NSImageView *IVFinaltexture;
	IBOutlet NSImageView *IVTemptexture;
	IBOutlet NSComboBox *CBOperation;
	IBOutlet tableviewLYR*	layerlist;	/* Layer array list */
	
	/* Controladores de los paneles "hijos"*/
	IBOutlet TGPlainControl *TGPlainCtrl;
	IBOutlet TGNoiseControl *TGNoiseCtrl;
	IBOutlet TGBlobControl *TGBlobCtrl;
	IBOutlet TGCelularControl *TGCelularCtrl;
	IBOutlet TGPlasmaControl *TGPlasmaCtrl;
	IBOutlet TGPerlinControl *TGPerlinCtrl;

	IBOutlet TGEfxCustomControl *TGEfxCustomCtrl;
    IBOutlet TGEfxColorizeControl *TGEfxColorizeCtrl;
    IBOutlet TGEfxGradBlurControl *TGEfxGradBlurCtrl;

	
	/* Common-C structures */
	CTextGen	*tg_final;
	CTextGen	*tg_temptext[TMP_TEXTURES];
//	c_gfx	gfx_Finaltext;					// Final texture
//	c_gfx	gfx_Temptext[TMP_TEXTURES];		// Temporary texture
	
}
- (void) hideAllPanels;

- (IBAction) showPlain:(id)sender;
- (void) GetPlainData:(T_PLAIN)t_data;

- (IBAction) showNoise:(id)sender;
- (void) GetNoiseData:(T_NOISE)t_data;

- (IBAction) showBlob:(id)sender;
- (void)GetBlobData:(T_BLOB)t_data;

- (IBAction) showCelular:(id)sender;
- (void)GetCelularData:(T_CELULAR)t_data;

- (IBAction) showPlasma:(id)sender;
- (void)GetPlasmaData:(T_PLASMA)t_data;

- (IBAction) showPerlin:(id)sender;
- (void)GetPerlinData:(T_PERLIN)t_data;

- (IBAction) showEfxCustom:(id)sender;
- (void)GetEfxCustom:(T_EFF_CUSTOM)t_data;

- (IBAction) showEfxColorize:(id)sender;
- (void)GetEfxColorize:(T_EFF_COLOR)t_data;

- (IBAction) showEfxGradBlur:(id)sender;
- (void)GetEfxGradBlur:(T_EFF_GRADBLUR)t_data;



- (IBAction) openZNTfile:(id)sender;
- (IBAction) saveZNTfile:(id)sender;
- (IBAction) saveTGAfile:(id)sender;

- (void) resetTemp:(id)sender;
- (void) renderFinal:(id)sender;
- (void) renderTemp:(id)sender;

- (void) DeleteAllLayers:(id)sender;
- (void) AddLayer:(id)sender;

- (void) AddEffect:(int)type;
- (IBAction) AddEffect_bw:(id)sender;
- (IBAction) AddEffect_r2polar:(id)sender;
- (IBAction) AddEffect_blur:(id)sender;
- (IBAction) AddEffect_mblur:(id)sender;
- (IBAction) AddEffect_edges1:(id)sender;
- (IBAction) AddEffect_edges2:(id)sender;
- (IBAction) AddEffect_sharpen1:(id)sender;
- (IBAction) AddEffect_sharpen2:(id)sender;
- (IBAction) AddEffect_sharpen3:(id)sender;
- (IBAction) AddEffect_emboss1:(id)sender;
- (IBAction) AddEffect_emboss2:(id)sender;
- (IBAction) AddEffect_mean1:(id)sender;
- (IBAction) AddEffect_mean2:(id)sender;
- (IBAction) AddEffect_custom3x3:(id)sender;
- (IBAction) AddEffect_colorize:(id)sender;
- (IBAction) AddEffect_gradblur:(id)sender;


- (void) DeleteLayer:(id)sender;
- (void) UpdateLayerList:(id)sender;
- (void) LayerUp:(id)sender;
- (void) LayerDown:(id)sender;
- (void) UpdateOperationFromLayer:(int)layer_num AndOperation:(int)operation;

- (void) LogFinalTexInfo;
- (void) LogTmpTexInfo;


#pragma mark Utilities

- (void) init_prepareText;

#pragma mark Accessors

// Llista de layers
//- (NSMutableArray*)layerList;
//- (void)setLayerList:(NSArray*)aValue;

@end


/////////////////////////////////////////////////////////////////
/// NSImage EXTRAS
/////////////////////////////////////////////////////////////////

@interface NSImage (Extras)
- (NSImage*)imageByScalingProportionallyToSize:(NSSize)aSize;
@end
