#include "MkNode.h"


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

void MkNode::ToString(std::ostringstream &stream)
{

}
