using System;
using System.Globalization;
using System.IO;
using System.Xml;
using System.Xml.Schema;

namespace Configuration
{
    public class XmlTool
    {

        public static XmlTool GetInstance { get; } = new XmlTool();

        private readonly XmlReaderSettings _settings = new XmlReaderSettings
        {
            ValidationType = ValidationType.Schema
        };

        public bool IsError { get; private set; } = false;
        public string Error { get; private set; } = "";

        public XmlDocument Doc { get; set; }
        public XmlElement Root { get; set; }


        private XmlTool()
        {
            IsError = false;
            Doc=new XmlDocument();
            Doc.AppendChild(Doc.CreateXmlDeclaration("1.0", "utf-8", null));

            Root = Doc.CreateElement("Configuration");
            Root.SetAttribute("xmlns", "http://tempuri.org/ConfigurationTemplate.xsd");
            Root.SetAttribute("xmlns:xsi", "http://tempuri.org/ConfigurationTemplate.xsd-instance");
            Root.SetAttribute("Date", DateTime.Now.ToString("s"));          
            Doc.AppendChild(Root);
            
        }

        //Error EventHandle
        private void ErrorEventHanlde(object sender, ValidationEventArgs e)
        {
            switch (e.Severity)
            {
                case XmlSeverityType.Error:
                    IsError = true;
                    Error += e.Message;
                    break;
                case XmlSeverityType.Warning:
                    break;
            }
        }

        
      
        public bool Load(string xmlFile)
        {
            IsError = false;
            if (File.Exists(xmlFile))
            {
                using (XmlReader rd = XmlReader.Create(xmlFile, _settings))
                {
                    try
                    {
                        while (rd.Read())
                        {
                            if (IsError) throw new XmlException("Fuck");
                        }
                        rd.Close();
                        Doc.RemoveAll();
                        Doc.Load(xmlFile);
                        Root = Doc.DocumentElement;

                        //Hook                    
                        if (Root!=null)
                        {
                            XmlNamespaceManager xnm = new XmlNamespaceManager(Doc.NameTable);
                            xnm.AddNamespace("hook", Root.GetAttribute("xmlns"));
                            FormFile.MeshCard = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:File/hook:IN/hook:Mesh", xnm);
                            FormFile.Project= Root.SelectSingleNode(
                                "/hook:Configuration/hook:File/hook:IN/hook:Project", xnm);
                            FormBasicFunction.OSCard = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:File/hook:OS", xnm);
                            FormMethod.MethodCard = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:Method", xnm);
                            FormFrequency.ParameterMod = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:EMCPara", xnm);
                            FormExcitation.ExcitationMod = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:Excitation", xnm);
                            FormSolution.SolutionMod = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:Solution", xnm);
                            FormRequest.RequestMod = (XmlElement)Root.SelectSingleNode(
                                "/hook:Configuration/hook:Request", xnm);
                        }
                        
                    }
                    catch (XmlException)
                    {
                        rd.Close();
                        return IsError;
                    }
                }
            }
            else IsError = true;
            return IsError;
        }

        public bool Validate()
        {
            Doc.Schemas=_settings.Schemas;
            IsError = false;
            Error = "";
            Doc.Validate(ErrorEventHanlde);
            return IsError;
        }

        //Help API
        public XmlElement AddElementWithText(string name, string text)
        {
            XmlElement result = Doc.CreateElement(name);
            result.InnerText = text;
            return result;
        }

        public XmlElement AddElementWithAttribute(string name, string attribute, string value)
        {
            XmlElement result = Doc.CreateElement(name);
            result.SetAttribute(attribute, value);
            return result;
        }
        
        //Template
        public void Initialization(string xsdFile, string namespaceurl = null)
        {
            //File-
            {
                XmlElement module = Doc.CreateElement("File");
                Root.AppendChild(module);
                //IN--
                module.AppendChild(FormFile.InitialTemplate());
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

            _settings.Schemas.Add(namespaceurl, xsdFile);
            _settings.ValidationEventHandler += ErrorEventHanlde;

            Doc.Schemas = _settings.Schemas;
        }
    }
}
