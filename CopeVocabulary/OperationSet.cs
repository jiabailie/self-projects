using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace CopeVocabulary
{
    public class OperationSet
    {
        //read the words which ready to be deal with
        public string[] getVocabulary(string path)
        {
            string tmp = "";

            List<string> word = new List<string>();
            StreamReader sr = new StreamReader(path);

            try
            {
                while (tmp != null)
                {
                    tmp = sr.ReadLine();
                    if (!string.IsNullOrEmpty(tmp))
                    {
                        word.Add(tmp);
                    }
                }

                sr.Close();
            }
            catch (IOException e)
            {
                throw new Exception(e.Message.ToString());
            }
            return word.ToArray();
        }

        //order the words and output to the certain file
        public string[] copeVocabulary(bool order, string path, string[] word)
        {
            int i = 0;
            int j = 0;
            string tmp = null;
            List<string> fWord = new List<string>();
            FileStream fs = new FileStream(path, FileMode.Create);
            StreamWriter sw = new StreamWriter(fs);

            try
            {
                Array.Sort(word);
                for (i = 0; i < word.Length; i++)
                {
                    if (i + 1 < word.Length && word[i].Length < word[j].Length)
                    {
                        for (j = 0; j < word[i].Length; j++)
                        {
                            if (word[i][j] != word[i + 1][j])
                            {
                                break;
                            }
                        }
                        if (j == word[i].Length)
                        {
                            continue;
                        }
                    }
                    tmp = word[i];
                    if (order)
                    {
                        tmp = (i + 1).ToString().PadLeft(2, '0') + ". " + word[i];
                        fWord.Add(tmp);
                    }
                    sw.WriteLine(tmp);
                }
                sw.Flush();
                sw.Close();
                fs.Close();
            }
            catch (Exception e)
            {
                throw new Exception(e.Message.ToString());
            }

            return fWord.ToArray();
        }
    }
}