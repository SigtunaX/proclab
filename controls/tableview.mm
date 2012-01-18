#import "tableview.h"
#import "../TGGUIControl.h"

/* for this example to work, the class must be set as:
 - data source for the table view
 - data source for the combo box
 - delegate for the table view */

@implementation tableviewLYR

/* create, randomly initialize and release the data structure for the data source
 we'll store the data in an array, each line is a dictionary with 3 entries:
 - pos contains the line number (just an ordinary column)
 - combo contains the current selection for the combo column
 - operationv contains an array with the values for the list */

-(id)init
{
	self = [super init];
	if(self)
		records = [[NSMutableArray arrayWithCapacity:25] retain];
	return self;
}

-(void)dealloc
{
	[records release];
	[super dealloc];
}

-(void)deleteLayerAtPos: (int)pos
{
	[records removeObjectAtIndex:pos];
	[TableView reloadData];
	NSLog(@"Layer list count (records internal): %lu", [records count]);
}

-(void)deleteallLayers
{
	[records removeAllObjects];
	[TableView reloadData];
	NSLog(@"Layer list count (records internal): %lu", [records count]);
}

-(void)addLayer:(NSString*)enabled image:(NSImage*)thumb operation:(int)oper_ind properties:(NSString*)props
{
	NSMutableArray *oper_list = [NSMutableArray arrayWithObjects: @"add", @"substract", @"average", @"multiply",@"minimum",@"maximum",@"amplitude",@"and",@"or", @"xor", nil];
	NSMutableDictionary *dic = [NSMutableDictionary dictionaryWithCapacity:4];
	[dic setObject:[NSString stringWithString:enabled] forKey:@"visible"];
	[dic setObject:[[NSImage alloc] initWithData:[thumb TIFFRepresentation]] forKey:@"thumb"];
	[dic setObject:oper_list forKey:@"operationv"];
	[dic setObject:[oper_list objectAtIndex:oper_ind] forKey:@"operation"];
	[dic setObject:[NSString stringWithString:props] forKey:@"properties"];
	[records addObject:dic];
	[TableView reloadData];
	NSLog(@"Layer list count (records internal): %lu", [records count]);
}

-(void)awakeFromNib
{
}

-(int)selectedRow
{
	return [TableView selectedRow];
}

-(void)selectRow:(int)row
{
	NSIndexSet * selSet = [NSIndexSet indexSetWithIndex:row];
	[TableView selectRowIndexes:selSet byExtendingSelection:NO];
}

-(int)numberOfRows
{
	return [TableView numberOfRows];
}

/* data source for the NSComboBoxCell
 it reads the data from the representedObject
 the cell is responsible to display and manage the list of options
 (we set representedObject in tableView:willDisplayCell:forTableColumn:row:)
 this is optional, the alternative is to enter a list of values in interface builder */

-(id)comboBoxCell:(NSComboBoxCell*)cell objectValueForItemAtIndex:(int)index
{
	NSArray *values = [cell representedObject];
	if(values == nil)
		return @"";
	else
		return [values objectAtIndex:index];
}

-(int)numberOfItemsInComboBoxCell:(NSComboBoxCell*)cell
{
	NSArray *values = [cell representedObject];
	if(values == nil)
		return 0;
	else
		return [values count];
}

-(NSUInteger)comboBoxCell:(NSComboBoxCell*)cell indexOfItemWithStringValue:(NSString*)st
{
	NSArray *values = [cell representedObject];
	if(values == nil)
		return NSNotFound;
	else
		return [values indexOfObject:st];
}

/* data source for the NSTableView
 the table is responsible to display and record the user selection
 (as opposed to the list of choices)
 this is required */

-(int)numberOfRowsInTableView:(NSTableView*)tableView
{
	return [records count];
}

-(id)tableView:(NSTableView*)tableView objectValueForTableColumn:(NSTableColumn*)tableColumn row:(int)index
{
	return [[records objectAtIndex:index] objectForKey:[tableColumn identifier]];
}

-(void)tableView:(NSTableView*)tableView setObjectValue:(id)value forTableColumn:(NSTableColumn*)tableColumn row:(int)index
{
	if(nil == value)
		value = @"";
	if([[tableColumn identifier] isEqual:@"operation"])
	{
		[[records objectAtIndex:index] setObject:value forKey:@"operation"];
		NSComboBoxCell *cell = [tableView selectedCell];
		[(TGGUIControl*)_parent UpdateOperationFromLayer:index AndOperation:[cell indexOfSelectedItem]];
	}
}

-(void)add:(NSTableView*)tableView setObjectValue:(id)value forTableColumn:(NSTableColumn*)tableColumn row:(int)index
{
	if(nil == value)
		value = @"";
	if([[tableColumn identifier] isEqual:@"operation"])
		[[records objectAtIndex:index] setObject:value forKey:@"operation"];
}

/* delegate for the NSTableView
 since there's only one combo box for all the lines, we need to populate it with the proper
 values for the line as set its selection, etc.
 this is optional, the alternative is to set a list of values in interface builder  */

-(void)tableView:(NSTableView*)tableView willDisplayCell:(id)cell forTableColumn:(NSTableColumn*)tableColumn row:(int)index
{
	if([[tableColumn identifier] isEqual:@"operation"] && [cell isKindOfClass:[NSComboBoxCell class]])
	{
		NSDictionary *dic = [records objectAtIndex:index];
		[cell setRepresentedObject:[dic objectForKey:@"operationv"]];
		[cell reloadData];
		[cell selectItemAtIndex:[self comboBoxCell:cell indexOfItemWithStringValue:[dic objectForKey:@"operation"]]];
		[cell setObjectValue:[dic objectForKey:@"operation"]];
	}
}

@end
