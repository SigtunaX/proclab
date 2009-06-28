//
//  TGGUIControl.h
//  proclab
//
//  Created by xphere on 19/04/08.
//  Copyright 2008 __MyCompanyName__. All rights reserved.
//

//#import <UIKit/UIKit.h
#import "TG_Controls/TGPlainControl.h"
#import "TG_Controls/TGCelularControl.h"
#import "c_structs.h"
#import "textgenlib/main.h"
#import "controls/tableview.h"

#define TMP_TEXTURES 1

@interface TGGUIControl : NSObject {
	/* Controles de la ventana principal */
	IBOutlet NSView *mainTGView;
	IBOutlet NSPanel *TGCelularPanel;
	IBOutlet NSImageView *IVFinaltexture;
	IBOutlet NSImageView *IVTemptexture;
	IBOutlet NSComboBox *CBOperation;
	IBOutlet NSTableView *TVLayerList;
	IBOutlet tableviewLYR*	layerlist;	/* Layer array list */
	
	/* Controladores de los paneles "hijos"*/
	IBOutlet TGPlainControl *TGPlainCtrl;
	IBOutlet TGCelularControl *TGCelularCtrl;
	//IBOutlet TGPlasmaControl *TGPlasmaCtrl;

	/* Common-C structures */
	CTextGen	*tg_final;
	CTextGen	*tg_temptext[TMP_TEXTURES];
//	c_gfx	gfx_Finaltext;					// Final texture
//	c_gfx	gfx_Temptext[TMP_TEXTURES];		// Temporary texture
	
}
- (void) hideAllPanels;
- (IBAction) showCelular:(id)sender;
- (IBAction) showPlain:(id)sender;
- (void) GetPlainData:(T_PLAIN)t_data;
- (void)GetCelularData:(T_CELULAR)t_data;
- (void) SaveToTGA:(id)sender;
- (void) AddText:(id)sender;
- (void) resetTemp:(id)sender;
- (void) renderFinal:(id)sender;
- (void) renderTemp:(id)sender;
- (void) DeleteAllLayers:(id)sender;
- (void) AddLayer:(id)sender;
- (void) DeleteLayer:(id)sender;
- (void) UpdateLayerList:(id)sender;
- (void) LogFinalTexInfo;
- (void) LogTmpTexInfo;
- (IBAction) openZNTfile:(id)sender;
- (void) UpdateOperationFromLayer:(int)layer_num AndOperation:(int)operation;


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
