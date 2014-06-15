/*
 * Copyright: (c) 2012. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */

package hid.logging
{
    import mx.formatters.DateFormatter;
    import mx.logging.AbstractTarget;
    import mx.logging.ILogger;
    import mx.logging.LogEvent;

    public class ConsoleTarget extends AbstractTarget
    {
        private static const SEPARATOR:String = " ";
        private static const DATE_TIME_FORMAT:String = "JJ:NN:SS.";

        private var dateFormatter:DateFormatter = new DateFormatter();

        public function ConsoleTarget()
        {
            dateFormatter.formatString = DATE_TIME_FORMAT;
        }

        override public function logEvent(event:LogEvent):void
        {
            var d:Date = new Date();
            var date:String = dateFormatter.format(d) + formatTime(d.getMilliseconds()) + SEPARATOR;
            var level:String = "[" + LogEvent.getLevelString(event.level) + "]" + SEPARATOR;
            var category:String = ILogger(event.target).category + SEPARATOR;
            var msg:String = date + level + category + event.message;
            trace(msg + "\n");
        }

        private function formatTime(num:Number):String
        {
            if (num < 10)
            {
                return "00" + num.toString();
            }
            else if (num < 100)
            {
                return "0" + num.toString();
            }
            else
            {
                return num.toString();
            }
        }
    }
}
