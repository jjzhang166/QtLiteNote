#pragma once

#include <vector>
#include <string>
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

std::pair<AnchorNode*, std::string> SyntaxMk(std::vector<std::string> lines);
void ReleaseAnchorNode(AnchorNode *);
