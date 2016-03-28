#pragma once

#include <ios>
#include <vector>
#include <memory>

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

    virtual void ToString(std::ostringstream &stream);
    virtual AnchorNodePtr CreateAnchor(int id);


private:
    std::vector<MkNodePtr> m_chNodes;
};

class HeadNode : public MkNode
{
public:
    HeadNode(int level, const std::string &text);

    void ToString(std::ostringstream &stream);
    AnchorNodePtr CreateAnchor(int id);

private:
    int m_headLevel;
    std::string m_headText;
    std::string m_headID;
};

class UlNode : public MkNode
{
public:
    void ToString(std::ostringstream &stream);
};

class OlNode : public MkNode
{
public:
    void ToString(std::ostringstream &stream);
};

class LiNode : public MkNode
{
public:
    LiNode(const std::string &text);

    void ToString(std::ostringstream &stream);

    void AppendText(const std::string &text);

};

class CodeNode : public MkNode
{
public:
    void ToString(std::ostringstream &stream);
    void AppendCode(const std::string &text);

private:
    std::vector<std::string> m_codeText;
};

class TextNode : public MkNode
{
public:
    TextNode(const std::string &text);

    void ToString(std::ostringstream &stream);
};