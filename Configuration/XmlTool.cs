using System;
using System.Globalization;
using System.IO;
using System.Xml;

namespace Configuration
{
    public class XmlTool
    {
        public static XmlDocument Doc { get; set; } = new XmlDocument();
        public static XmlElement Root { get; set; }

        //Help API
        public static XmlElement AddElementWithText(string name, string text)
        {
            XmlElement result = Doc.CreateElement(name);
            result.InnerText = text;
            return result;
        }

        public static XmlElement AddElementWithAttribute(string name, string attribute,string value)
        {
            XmlElement result = Doc.CreateElement(name);
            result.SetAttribute(attribute, value);
            return result;
        }

        public static void Load(string filename)
        {
            if (File.Exists(filename))
            {
               Doc.RemoveAll();
               Doc.Load(filename);
               Root= Doc.DocumentElement;
            }
        }

        //Initial with Template
        public static void Initial()
        {
            Doc.AppendChild(Doc.CreateXmlDeclaration("1.0", "utf-8", null));
            
            Root = AddElementWithAttribute("Configuration", "Date",
                DateTime.Now.ToString(CultureInfo.InvariantCulture));
            Doc.AppendChild(Root);

            //File-
            {
                XmlElement module = Doc.CreateElement("File");
                Root.AppendChild(module);
                //IN--
                {
                    XmlElement card = AddElementWithAttribute("IN", "Validate", "1");
                    module.AppendChild(card);
                    //mesh---
                    card.AppendChild(FormFile.InitialTemplate());
                    //project---
                    XmlElement node = Doc.CreateElement("Project");
                    card.AppendChild(node);
                    node.AppendChild(AddElementWithText("Name", "test"));
                    node.AppendChild(AddElementWithText("Directory", @"E:\ZMC\Code\C_program\MUL\SourceData"));
                    node.AppendChild(AddElementWithText("Report", @"E:\ZMC\Code\C_program\MUL\SourceData\test.out"));
                }
                //OS-- 
                module.AppendChild(FormBasicFunction.InitialTemplate());
            }           
            //Method-
            Root.AppendChild(FormMethod.InitialTemplate());
            //EMCParameters-
            Root.AppendChild(FormFrequency.InitialTemplate());
            //Excitation-
            Root.AppendChild(FormExcitation.InitialTemplate());
            //Solution-
            Root.AppendChild(FormSolution.InitialTemplate());
            //Request-
            Root.AppendChild(FormRequest.InitialTemplate());
        }

    }
}
