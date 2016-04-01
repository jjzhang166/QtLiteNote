#pragma once

#include <sstream>
#include <vector>
#include <utility>
#include "MkNode.h"

class MkNode;
typedef std::shared_ptr<MkNode> MkNodePtr;

class MkNode
{
public:
    MkNode();
    ~MkNode();

    virtual void ToString(std::stringstream &stream);
    virtual AnchorNode* CreateAnchor(int id);

    void appendCh(MkNodePtr ch);

public:
    std::vector<MkNodePtr> m_chNodes;
};

class HeadNode : public MkNode
{
public:
    HeadNode(int level, const std::string &text);

    void ToString(std::stringstream &stream);
    AnchorNode* CreateAnchor(int id);

private:
    int m_headLevel;
    std::string m_headText;
    std::string m_headID;
};

class UlNode : public MkNode
{
public:
    void ToString(std::stringstream &stream);
};

class OlNode : public MkNode
{
public:
    void ToString(std::stringstream &stream);
};

class LiNode : public MkNode
{
public:
    LiNode(MkNodePtr txtNode);

    void ToString(std::stringstream &stream);

    void AppendText(const std::string &text);
};

class CodeNode : public MkNode
{
public:
    void ToString(std::stringstream &stream);
    void AppendCode(const std::string &text);

private:
    std::vector<std::string> m_codeText;
};

class TextNode : public MkNode
{
public:
    TextNode(const std::string &text, const std::string &file_dir);

    void ToString(std::stringstream &stream);

private:
    std::string m_text;
	std::string m_file_dir;
};

//////////////////////////////////////////////
// MkContent
class MkContent
{
public:
	MkContent(const std::string &file_dir);
    void AppendTop(MkNodePtr node);
    void AppendLi(MkNodePtr li);
    void LiAppendCodeNode(MkNodePtr codeNode);
    void LiAppendCodeContent(const std::string &text);
    void LiAppendText(const std::string &text);
    void AppendCodeContent(const std::string &text);
    void ToString(std::stringstream &stream);

    AnchorNode* CreateMkLevel();

private:
    AnchorNode* CreateTree(std::vector<AnchorNode*> &heads);


private:
    std::vector<MkNodePtr> m_tops;
	std::string m_file_dir;
};

//////////////////////////////////////////////
// MkSyntax
class MkSyntax
{
public:
    MkSyntax(std::vector<std::string> &lines, const std::string &file_dir);

    void Analyse();
	void ToString(std::stringstream &stream);
	AnchorNode* GetMkContent();

private:
	bool ActionTop(int i, int &stat);
    void ActionUl(int &i, int &stat);
    void ActionOl(int &i, int &stat);
    void ActionUlCode(int &i, int &stat);
    void ActionOlCode(int &i, int &stat);
    std::pair<int, std::string> GetHeadLevel(int i);
	std::pair<bool, std::string> SplitStartItem(int i, std::string pat);
	std::pair<bool, std::string> GetUlItem(int i);
	std::pair<bool, std::string> GetOlItem(int i);
    std::pair<bool, std::string> GetTabStart(int i);
    bool IsCodeLine(int i);
    bool IsTabCodeLine(int i);

private:
    std::vector<std::string> m_lines;
	std::string m_file_dir;
    MkContent m_content;
};

