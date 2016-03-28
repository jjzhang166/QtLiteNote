#include "MkNode.h"
#include <stdio.h>
#include <regex>


void AnchorNode::AppendCh(AnchorNodePtr node)
{
    m_children.push_back(node);
}


MkNode::MkNode()
{
}


MkNode::~MkNode()
{
}

void MkNode::ToString(std::stringstream &stream)
{

}

AnchorNodePtr MkNode::CreateAnchor(int id)
{
   AnchorNodePtr e; 
   return e;
}

void MkNode::appendCh(MkNodePtr ch)
{
    m_chNodes.push_back(ch);
}

void HeadNode::ToString(std::stringstream &stream)
{
    char str[1024];
    sprintf(str, "<a id=\"%s\"></a>", m_headID.c_str());
    stream << str;
    stream << "<h" << m_headLevel <<">";
    stream << m_headText;
    stream << "</h" << m_headLevel << ">";
}

AnchorNodePtr HeadNode::CreateAnchor(int id)
{
    char str[1024];
    sprintf(str, "md_%d", id);
    m_headID = std::string(str);

    AnchorNodePtr a(new AnchorNode);
    a->m_level = m_headLevel;
    a->m_anchorID = m_headID;
    a->m_name = m_headText;
    return a;
}

void UlNode::ToString(std::stringstream &stream)
{
    stream << "<ul>";
    for (int i = 0; i < m_chNodes.size(); ++i) {
        m_chNodes[i]->ToString(stream);
    }
    stream << "</ul>";
    stream << "\n";
}

void OlNode::ToString(std::stringstream &stream)
{
   stream << "<ol>";
   for (int i = 0; i < m_chNodes.size(); ++i) {
       m_chNodes[i]->ToString(stream);
   }
   stream << "</ol>";
   stream << "\n";
}

void LiNode::ToString(std::stringstream &stream)
{
    stream << "<li>";
    for (int i = 0; i < m_chNodes.size(); ++i) {
        m_chNodes[i]->ToString(stream);
    }
    stream << "</li>";
}

void LiNode::AppendText(const std::string &text)
{
    MkNodePtr txtNode(new TextNode(text));
    m_chNodes.push_back(txtNode);
}

void CodeNode::ToString(std::stringstream &stream)
{
    stream << "<pre><code>";
    for (int i = 0; i < m_codeText.size(); ++i) {
        stream << m_codeText[i];
        stream << "<br/>";
    }
    stream << "</pre></code>";
}

void CodeNode::AppendCode(const std::string &text)
{
    m_codeText.push_back(text);
}

TextNode::TextNode(const std::string &text)
    : m_text(text)
{

}

void TextNode::ToString(std::stringstream &stream)
{
    stream << m_text;
    stream << "<br/>";
}
////////////////////////////////////////////

void MkContent::AppendTop(MkNodePtr node)
{
    m_tops.push_back(node);
}

void MkContent::AppendLi(MkNodePtr li)
{
    m_tops.back()->appendCh(li);
}

void MkContent::LiAppendCodeNode(MkNodePtr codeNode)
{
    m_tops.back()->m_chNodes.back()->appendCh(codeNode);
}

void MkContent::LiAppendCodeContent(MkNodePtr text)
{
    m_tops.back()->m_chNodes.back()->m_chNodes.back()->appendCh(text);
}

void MkContent::LiAppendText(const std::string &text)
{
    MkNodePtr ptr = m_tops.back()->m_chNodes.back();
    MkNode *p = ptr.get();
    LiNode *li = (LiNode*)p;
    li->AppendText(text);
}

void MkContent::AppendCodeContent(const std::string &text)
{
    MkNodePtr ptr = m_tops.back();
    CodeNode *code = (CodeNode*)ptr.get();
    code->AppendCode(text);
}

void MkContent::ToString(std::stringstream &stream)
{
    for (int i = 0; i < m_tops.size(); ++i) {
        m_tops[i]->ToString(stream);
    }
}

void MkContent::CreateMkLevel()
{
    std::vector<AnchorNodePtr> heads;
    int headID = 0;
    for (int i = 0; i < m_tops.size(); ++i) {
        AnchorNodePtr a = m_tops[i]->CreateAnchor(headID);
        if (a) {
            heads.push_back(a);
            ++headID;
        }
    }
    AnchorNodePtr n = CreateTree(heads);
}

AnchorNodePtr MkContent::CreateTree(std::vector<AnchorNodePtr> &heads)
{
    std::vector<AnchorNodePtr> cc;
    cc.resize(7, AnchorNodePtr());
    
    for (int i = 0; i < heads.size(); ++i) {
        int level = heads[i]->m_level;
        AnchorNodePtr an;
        int ii = level - 1;
        while (ii >= 0) {
            if (cc[ii]) {
                an = cc[ii];
                break;
            }
            --ii;
        }
        an->AppendCh(heads[i]);
        cc[level] = heads[i];
        for (int ii = i; ii < 7; ++ii) {
            AnchorNodePtr e;
            cc[ii] = e;
        }
    }
    return cc[0];
}

//////////////////////////////////////////////
// MkSyntax


MkSyntax::MkSyntax(std::vector<std::string> &lines)
: m_lines(lines)
{
    
}

void MkSyntax::Analyse()
{
    
}

void ActionTop(int i, int &stat)
{

}

void ActionUl(int i, int &stat)
{

}

void ActionOl(int i, int &stat)
{

}

void ActionUlCode(int i, int &stat)
{

}

void ActionOlCode(int i, int &stat)
{

}












