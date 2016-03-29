#include "MkNodePri.h"
#include <stdio.h>
#include <regex>
#include <algorithm>
#include <functional>

inline std::string& ltrim(std::string &ss)
{
	int(*pf)(int) = isspace;
	std::string::iterator p = std::find_if(ss.begin(), ss.end(), std::not1(std::ptr_fun(pf)));
	ss.erase(ss.begin(), p);
	return ss;
}

inline std::string& rtrim(std::string &ss)
{
	int(*pf)(int) = isspace;
	std::string::reverse_iterator p = std::find_if(ss.rbegin(), ss.rend(), std::not1(std::ptr_fun(pf)));
	ss.erase(p.base(), ss.end());
	return ss;
}

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

HeadNode::HeadNode(int level, const std::string &text)
: m_headLevel(level),
m_headText(text)
{

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

LiNode::LiNode(const std::string &text)
{
	MkNodePtr txtNode(new TextNode(text));
	m_chNodes.push_back(txtNode);
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

void MkContent::LiAppendCodeContent(const std::string &text)
{
	MkNodePtr ptr = m_tops.back()->m_chNodes.back()->m_chNodes.back();
	CodeNode *code = (CodeNode*)ptr.get();
	code->AppendCode(text);
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
	cc[0] = AnchorNodePtr(new AnchorNode);
    
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
        for (int ii = level+1; ii < 7; ++ii) {
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
	int stat = 0;
	int i = -1;
	while (1) {
		++i;
		printf("lineNum: %d\n", i);

		if (m_lines.size() == i)
			break;

		bool haveAct = false;

		if (stat == 0) {
			haveAct = ActionTop(i, stat);

		} else if (stat == 1) {
			ActionUl(i, stat);
			haveAct = true;

		} else if (stat == 2) {
			ActionOl(i, stat);
			haveAct = true;

		} else if (stat == 3) {
			if (IsCodeLine(i)) {
				stat = 0;
			} else {
				m_content.AppendCodeContent(m_lines[i]);
			}
			haveAct = true;

		} else if (stat == 13) {
			ActionUlCode(i, stat);
			haveAct = true;

		} else if (stat == 23) {
			ActionOlCode(i, stat);
			haveAct = true;

		} 
		if (!haveAct) {
			MkNodePtr txtNode(new TextNode(m_lines[i]));
			m_content.AppendTop(txtNode);
		}
	}
}

void MkSyntax::GetMkContent()
{
	m_content.CreateMkLevel();
}

void MkSyntax::ToString(std::stringstream &stream)
{
	m_content.ToString(stream);
}

bool MkSyntax::ActionTop(int i, int &stat)
{
	std::pair<int, std::string> head = GetHeadLevel(i);
	int headLevel = head.first;
	std::string text = head.second;

	if (headLevel >= 1 && headLevel <= 6 && !text.empty()) {
		MkNodePtr headNode(new HeadNode(headLevel, text));
		m_content.AppendTop(headNode);
		return true;
	}
	std::pair<bool, std::string> r = GetUlItem(i);
	bool isUl = r.first;
	text = r.second;
	if (isUl) {
		MkNodePtr ul(new UlNode());
		MkNodePtr li(new LiNode(text));
		ul->appendCh(li);
		m_content.AppendTop(ul);
		stat = 1;
		return true;
	}
	r = GetOlItem(i);
	bool isOl = r.first;
	text = r.second;
	if (isOl) {
		MkNodePtr ol(new OlNode);
		MkNodePtr li(new LiNode(text));
		ol->appendCh(li);
		m_content.AppendTop(ol);
		stat = 2;
		return true;
	}
	if (IsCodeLine(i)) {
		MkNodePtr code(new CodeNode);
		m_content.AppendTop(code);
		stat = 3;
		return true;
	}
	return false;
}

void MkSyntax::ActionUl(int &i, int &stat)
{
	std::pair<bool, std::string> r = GetUlItem(i);
	bool isUl = r.first;
	std::string &text = r.second;
	if (isUl) {
		MkNodePtr li(new LiNode(text));
		m_content.AppendLi(li);
		return;
	}
	if (IsTabCodeLine(i)) {
		MkNodePtr code(new CodeNode);
		m_content.LiAppendCodeNode(code);
		stat = 13;
		return;
	}

	r = GetTabStart(i);
	bool isTab = r.first;
	if (isTab) {
		m_content.LiAppendText(text);
	} else {
		stat = 0;
		--i;
	}
}

void MkSyntax::ActionOl(int &i, int &stat)
{
	std::pair<bool, std::string> r = GetOlItem(i);

	if (r.first) {
		MkNodePtr li(new LiNode(r.second));
		m_content.AppendLi(li);
		return;
	}
	if (IsTabCodeLine(i)) {
		MkNodePtr code(new CodeNode);
		m_content.LiAppendCodeNode(code);
		stat = 23;
		return;
	}
	r = GetTabStart(i);
	bool isTab = r.first;

	if (isTab) {
		m_content.LiAppendText(r.second);
	} else {
		stat = 0;
		--i;
	}
}

void MkSyntax::ActionUlCode(int &i, int &stat)
{
	if (IsTabCodeLine(i)) {
		stat = 1;
		return;
	}

	if (m_lines[i].empty()) {
		m_content.LiAppendCodeContent(" ");
		return;
	}

	std::pair<bool, std::string> r = GetTabStart(i);
	if (r.first) {
		m_content.LiAppendCodeContent(r.second);
	} else {
		stat = 1;
		--i;
	}
}

void MkSyntax::ActionOlCode(int &i, int &stat)
{
	if (IsTabCodeLine(i)) {
		stat = 2;
		return;
	}
	if (m_lines[i].empty()) {
		m_content.LiAppendCodeContent(" ");
		return;
	}

	std::pair<bool, std::string> r = GetTabStart(i);
	bool isTab = r.first;
	std::string &text = r.second;
	if (isTab) {
		m_content.LiAppendCodeContent(text);
	} else {
		stat = 2;
		--i;
	}
		
}

std::pair<int, std::string> MkSyntax::GetHeadLevel(int i)
{
	std::string &line = m_lines[i];
	std::regex pat("^#+");
	std::smatch match;
	if (std::regex_search(line, match, pat)) {
		std::ssub_match suf = match.suffix();

		std::string h(line.begin(), suf.first);
		std::string after(suf.first, line.end());
		//after = ltrim(after);

		return std::make_pair(h.size(), after);
	}
	return std::make_pair(0, "");
}

std::pair<bool, std::string> MkSyntax::GetUlItem(int i)
{
	return SplitStartItem(i, "^\\* ");
}

std::pair<bool, std::string> MkSyntax::GetOlItem(int i)
{
	return SplitStartItem(i, "^[0-9]+\\. ");
}

std::pair<bool, std::string> MkSyntax::SplitStartItem(int i, std::string pat)
{
	std::string &line = m_lines[i];
	std::regex reg(pat);
	std::smatch match;
	if (std::regex_search(line, match, reg)) {
		std::ssub_match suf = match.suffix();
		std::string after(suf.first, line.end());
		//after = ltrim(after);
		return std::make_pair(true, after);
	}
	return std::make_pair(false, "");
}

std::pair<bool, std::string> MkSyntax::GetTabStart(int i)
{
	std::string &line = m_lines[i];
	if (line.size() >= 2 && line[0] == '\t') {
		return std::make_pair(true, line.substr(1));
	}
	if (line.size() >= 5 && line.substr(0, 4) == "    ") {
        return std::make_pair(true, line.substr(4));
    }
    return std::make_pair(false, "");
}


bool MkSyntax::IsCodeLine(int i)
{
    std::string &line = m_lines[i];
    if (line == "```") {
        return true;     
    }
    return false;
}
bool MkSyntax::IsTabCodeLine(int i)
{
    std::string &line = m_lines[i];
    if (line.size() == 4) {
        if (line[0] == '\t' && line.substr(1, 3) == "```") {
            return true;     
        }
    }
    if (line.size() == 7) {
		if (line.substr(0, 4) == "    " && line.substr(4, 3) == "```") {
			return true;
		}
    }
	return false;
}

std::pair<AnchorNode*, std::string> SyntaxMk(std::vector<std::string> lines)
{

	AnchorNode *node;
	return std::make_pair(node, "");
}

void ReleaseAnchorNode(AnchorNode *)
{
	
}

