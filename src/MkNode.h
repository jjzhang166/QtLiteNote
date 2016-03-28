#pragma once

#include <sstream>
#include <vector>
#include <memory>
#include <utility>

class AnchorNode;
typedef std::shared_ptr<AnchorNode> AnchorNodePtr;

class AnchorNode
{
public:
    void AppendCh(AnchorNodePtr node);

public:
    int m_level;
    std::string m_anchorID;
    std::string m_name;
    std::vector<AnchorNodePtr> m_children;
};

class MkNode;
typedef std::shared_ptr<MkNode> MkNodePtr;

class MkNode
{
public:
    MkNode();
    ~MkNode();

    virtual void ToString(std::stringstream &stream);
    virtual AnchorNodePtr CreateAnchor(int id);

    void appendCh(MkNodePtr ch);

public:
    std::vector<MkNodePtr> m_chNodes;
};

class HeadNode : public MkNode
{
public:
    HeadNode(int level, const std::string &text);

    void ToString(std::stringstream &stream);
    AnchorNodePtr CreateAnchor(int id);

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
    LiNode(const std::string &text);

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
    TextNode(const std::string &text);

    void ToString(std::stringstream &stream);

private:
    std::string m_text;
};

//////////////////////////////////////////////
// MkContent
class MkContent
{
public:
    void AppendTop(MkNodePtr node);
    void AppendLi(MkNodePtr li);
    void LiAppendCodeNode(MkNodePtr codeNode);
    void LiAppendCodeContent(MkNodePtr text);
    void LiAppendText(const std::string &text);
    void AppendCodeContent(const std::string &text);
    void ToString(std::stringstream &stream);

    void CreateMkLevel();


private:
    AnchorNodePtr CreateTree(std::vector<AnchorNodePtr> &heads);


private:
    std::vector<MkNodePtr> m_tops;
};

//////////////////////////////////////////////
// MkSyntax
class MkSyntax
{
public:
    MkSyntax(std::vector<std::string> &lines);

    void Analyse();

private:
    void ActionTop(int i, int &stat);
    void ActionUl(int i, int &stat);
    void ActionOl(int i, int &stat);
    void ActionUlCode(int i, int &stat);
    void ActionOlCode(int i, int &stat);
    std::pair<bool, std::string> GetHeadLevel(int i);
    std::pair<bool, std::string> GetUlItem(int i);
    std::pair<bool, std::string> GetOlItem(int i);
    std::pair<bool, std::string> GetTabStart(int i);
    bool IsCodeLine(int i);
    bool IsTabCodeLine(int i);

private:
    std::vector<std::string> m_lines;
    MkContent m_content;
};























