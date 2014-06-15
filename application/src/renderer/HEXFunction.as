/*
 * Copyright: (c) 2014. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */

package renderer
{
    import mx.controls.dataGridClasses.DataGridColumn;

    public class HEXFunction
    {
        private var dataField:String;
        
        public function HEXFunction(dataField:String)
        {
            this.dataField = dataField;
        }

        public function labelFunction(item:Object, column:DataGridColumn):String
        {
            var result:String = "";
            if(item && item.hasOwnProperty(dataField))
            {
                result = "0x" + uint(item[dataField]).toString(16).toUpperCase();
            }
            else
            {
                result = "0x0";
            }
            return result;
        }
    }
}
