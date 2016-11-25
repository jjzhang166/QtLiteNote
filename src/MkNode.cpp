#include "MkNodePri.h"
#include <stdio.h>
#include <regex>
#include <algorithm>
#include <functional>

std::string ltrim(const std::string &str)
{
	if (str.empty()) {
		return str;
	}
    size_t index = str.find_first_not_of(" \n\r\t");
	if (index >= 0 && index < str.length()) {
		return str.substr(index);
	}
	return str;
}

std::string rtrim(const std::string &str)
{
    size_t index = str.find_last_not_of(" \n\r\t");
	//TODO:
	return str;
}

std::string HtmlTagFilter(const std::string &str)
{
	std::string dst = str;
	// & -> &amp;
	// < -> &lt;
	// > -> &gt;
	// " -> &quot;
	std::regex reg1("&");
	dst = std::regex_replace(dst, reg1, "&amp;");

	std::regex reg2("<");
	dst = std::regex_replace(dst, reg2, "&lt;");

	std::regex reg3(">");
	dst = std::regex_replace(dst, reg3, "&gt;");

	std::regex reg4("\"");
	dst = std::regex_replace(dst, reg4, "&quot;");

	return dst;
}

std::string HtmlImageFilter(const std::string &str, const std::string &file_dir)
{
	std::regex pat("!\\[\\]\\((.*)\\)");
	std::smatch match;
	if (std::regex_search(str, match, pat)) {

		std::string s(match[1]);
		//printf("%s\n", s.c_str());
		char img[1024];
		sprintf(img, "<img src=\"file:///%s/%s\"/>", file_dir.c_str(), s.c_str());
		return img;
	}
	return str;
}

AnchorNode::AnchorNode()
	: m_level(0)
{

}

void AnchorNode::AppendCh(AnchorNode* node)
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

AnchorNode* MkNode::CreateAnchor(int id)
{
	return NULL;
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

AnchorNode* HeadNode::CreateAnchor(int id)
{
    char str[1024];
    sprintf(str, "md_%d", id);
    m_headID = std::string(str);

    AnchorNode* a = new AnchorNode;
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

LiNode::LiNode(MkNodePtr txtNode)
{
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

void CodeNode::ToString(std::stringstream &stream)
{
    stream << "<pre><code>";
    for (int i = 0; i < m_codeText.size(); ++i) {
        stream << HtmlTagFilter(m_codeText[i]);
        stream << "<br/>";
    }
    stream << "</pre></code>";
}

void CodeNode::AppendCode(const std::string &text)
{
    m_codeText.push_back(text);
}

TextNode::TextNode(const std::string &text, const std::string &file_dir)
    : m_text(text),
	m_file_dir(file_dir)
{

}

void TextNode::ToString(std::stringstream &stream)
{
	std::string text = HtmlTagFilter(m_text);
	text = HtmlImageFilter(text, m_file_dir);
	stream << text;
    stream << "<br/>";
}
////////////////////////////////////////////

MkContent::MkContent(const std::string &file_dir)
	: m_file_dir(file_dir)
{
}
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
    MkNodePtr txtNode(new TextNode(text, m_file_dir));
	li->appendCh(txtNode);
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

AnchorNode* MkContent::CreateMkLevel()
{
    std::vector<AnchorNode*> heads;
    int headID = 0;

    for (int i = 0; i < m_tops.size(); ++i) {
        AnchorNode* a = m_tops[i]->CreateAnchor(headID);

        if (a) {
            heads.push_back(a);
            ++headID;
        }
    }
    AnchorNode* n = CreateTree(heads);
	return n;
}

AnchorNode* MkContent::CreateTree(std::vector<AnchorNode*> &heads)
{
    std::vector<AnchorNode*> cc;
    cc.resize(7, NULL);
	cc[0] = new AnchorNode;
    
    for (int i = 0; i < heads.size(); ++i) {

        int level = heads[i]->m_level;
		AnchorNode *an;
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
			cc[ii] = NULL;
        }
    }
    return cc[0];
}

//////////////////////////////////////////////
// MkSyntax

MkSyntax::MkSyntax(std::vector<std::string> &lines, const std::string &file_dir)
: m_lines(lines),
m_file_dir(file_dir),
m_content(file_dir)
{
    
}

void MkSyntax::Analyse()
{
	int stat = 0;
	int i = -1;
	while (1) {
		++i;
		//printf("lineNum: %d\n", i);

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
            const std::string &line = m_lines[i];
            if (line.length() > 0) {
                MkNodePtr txtNode(new TextNode(line, m_file_dir));
                m_content.AppendTop(txtNode);
            }
		}
	}
}

AnchorNode* MkSyntax::GetMkContent()
{
	return m_content.CreateMkLevel();
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
		MkNodePtr txtNode(new TextNode(text, m_file_dir));
		MkNodePtr li(new LiNode(txtNode));
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
		MkNodePtr txtNode(new TextNode(text, m_file_dir));
		MkNodePtr li(new LiNode(txtNode));
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
		MkNodePtr txtNode(new TextNode(text, m_file_dir));
		MkNodePtr li(new LiNode(txtNode));
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
		MkNodePtr txtNode(new TextNode(r.second, m_file_dir));
		MkNodePtr li(new LiNode(txtNode));
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
        std::ssub_match pre = match.prefix();

        std::string h(pre.first, suf.first);
        std::string after(suf.first, suf.second);

        after = ltrim(after);

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
        std::string after(suf.first, suf.second);
        after = ltrim(after);
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

const char *css_templ = "<html>"
"<head>"
"    <meta http-equiv=\"content-type\" content=\"text/html;charset=UTF-8\">"
"    <style type=\"text/css\">"
"        body { color: #444444; font-size:%dpt; line-height:1.4; word-wrap: break-word; font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1; }"
"        h1, h2, h3, h4 { color: #111111; font-weight: bold; margin-top:3pt;margin-bottom:3pt;}"
"        h1 { font-size: %dpt; border-bottom: 1pt solid; color: #D6615C; }"
"        h2 { font-size: %dpt; font-weight: bold; color: #2F517B; }"
"        h3 { font-size: %dpt; font-style: italic; }"
"        h4 { font-size: %dpt; font-style: italic; }"
"        h5 { font-size: %dpt; font-style: italic; }"
"        h6 { font-size: %dpt; font-style: italic; }"
"        a { color: #0099ff; margin: 0; padding: 0; vertical-align: baseline; }"
"        a:link, a:visited { text-decoration: none; }"
"        a:hover { text-decoration: underline; }"
"        br {display:inline;}"
"        pre { padding: 4pt; max-width: 100%%white-space; line-height: 1.5; font-size: %dpt; border: 1pt solid #ddd; background-color: #f7f7f7;  }"
"        code { font-family: DejaVu Sans Mono, \\5FAE\\8F6F\\96C5\\9ED1; line-height: 1.5; font-size: %dpt; background-color: #f7f7f7; }"
"        td, th { border: 1pt solid #ccc; padding: 5pt; }"
"        aside { display: block; float: right; width: 100%%; }"
"        blockquote { border-left: .5em solid #40AA53; padding: 0 2em; margin-left: 0; max-width: 100%%; }"
"        blockquote cite { font-size: %dpt; line-height: 1.5; color: #bfbfbf; }"
"        blockquote p { color: #666; max-width: 100%%; }"
"        table { border-spacing: 0; border: 1pt; solid #ccc; }"
"    </style>"
"</head>"
"<body><p/>";

// %d

std::pair<std::string, AnchorNode*> SyntaxMk(std::vector<std::string> &lines, const std::string &file_dir, int font_size)
{
    size_t len = strlen(css_templ);
    char *str = new char[len+1024];
	int len2 = strlen(str);
    int diff = 1;
    sprintf(str, css_templ,
		font_size, font_size+diff*5, font_size+diff*4, font_size+diff*3, font_size+diff*2, font_size+diff, font_size,
            font_size-diff, font_size-diff, font_size-diff);
    
	MkSyntax syn(lines, file_dir);
	syn.Analyse();

	AnchorNode *node = syn.GetMkContent();
	std::stringstream stream;
	stream << str;
    delete [] str;
	syn.ToString(stream);
	stream << "</body></html>";
	return std::make_pair(stream.str(), node);
}

void ReleaseAnchorNode(AnchorNode *node)
{
	if (node) {
		for (int i = 0; i < node->m_children.size(); ++i) {
			ReleaseAnchorNode(node->m_children[i]);
		}
		delete node;
	}
}
