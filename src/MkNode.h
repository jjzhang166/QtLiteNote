#pragma once

#include <vector>
#include <string>
#include <memory>

class AnchorNode;
typedef std::shared_ptr<AnchorNode> AnchorNodePtr;

class AnchorNode
{
public:
	AnchorNode();
    void AppendCh(AnchorNode* node);

public:
    int m_level;
    std::string m_anchorID;
    std::string m_name;
    std::vector<AnchorNode*> m_children;
};

std::pair<std::string, AnchorNode*> SyntaxMk(std::vector<std::string> &lines, const std::string &file_dir, int font_size);
void ReleaseAnchorNode(AnchorNode *node);
