/*
 * Copyright: (c) 2014. Turtsevich Alexander
 *
 * Licensed under the MIT license: http://www.opensource.org/licenses/mit-license.html
 */

package hid.logging
{
    import mx.controls.TextArea;
    import mx.formatters.DateFormatter;
    import mx.logging.AbstractTarget;
    import mx.logging.ILogger;
    import mx.logging.LogEvent;
    import mx.logging.LogEventLevel;

    public class TextTarget extends AbstractTarget
    {
        private static const SEPARATOR:String = " ";
        private static const DATE_TIME_FORMAT:String = "JJ:NN:SS.";

        private var dateFormatter:DateFormatter = new DateFormatter();

        private var output:TextArea = null;

        public function TextTarget(output:TextArea)
        {
            dateFormatter.formatString = DATE_TIME_FORMAT;
            this.output = output;
        }

        override public function logEvent(event:LogEvent):void
        {
            if (output)
            {
                var d:Date = new Date();
                var date:String = dateFormatter.format(d) + formatTime(d.getMilliseconds()) + SEPARATOR;
                var level:String = "[" + LogEvent.getLevelString(event.level) + "]" + SEPARATOR;
                var category:String = ILogger(event.target).category + SEPARATOR;
                var msg:String = date + level + category + event.message;
                var formatted:String = applyColor(msg, event.level);
                output.htmlText += formatted + "<br>";
                output.callLater(output.callLater, [scrollDown, []]);
            }
        }

        private function scrollDown():void
        {
            output.verticalScrollPosition = output.maxVerticalScrollPosition;
        }

        private function applyColor(msg:String, level:int):String
        {
            switch (level)
            {
                case LogEventLevel.ERROR:
                    return "<font color='#FF0000'>" + msg + "</font>";
                case LogEventLevel.WARN:
                    return "<font color='#0000FF'>" + msg + "</font>";
                case LogEventLevel.DEBUG:
                    return "<font color='#BBBBBB'>" + msg + "</font>";
            }
            return msg;
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
