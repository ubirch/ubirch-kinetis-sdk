/*
 * Copyright (c) 2014, Freescale Semiconductor, Inc.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _EMBEDDED_RPC__ASTNODE_H_
#define _EMBEDDED_RPC__ASTNODE_H_

#include <typeinfo>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "smart_ptr.h"
#include "Token.h"

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpcgen
{
/*!
 * @brief Homogeneous AST node class.
 *
 * This class is meant to represent arbitrary AST nodes. Each node optionally has a Token object
 * associated with it. It has an ordered list of child nodes and a pointer to its parent. Named
 * attribute values can be set on a node. The attribute values are members of the Value class
 * hierarchy of boxed types.
 */
class AstNode
{
public:
    typedef std::map<std::string, Value *> attribute_map_t;

    typedef std::vector<AstNode *> child_list_t;
    typedef child_list_t::iterator iterator;
    typedef child_list_t::const_iterator const_iterator;

    /*!
     * @brief This function is constructor of AstNode class.
     *
     * This function set parent AstNode variable for this object to nullptr
     * and token variable for this object to given token.
     *
     * @param[in] token Constant reference to token.
     *
     * @see AstNode::AstNode(Token * token, AstNode * parent)
     * @see AstNode::AstNode(const AstNode & other)
     */
    AstNode(const Token &token)
    : m_token(token)
    , m_parent(nullptr)
    {
    }

    /*!
     * @brief This function is constructor of AstNode class.
     *
     * This function set parent AstNode variable for this object to given node
     * and token variable for this object to given token.
     *
     * @param[in] token Pointer to given token.
     * @param[in] parent Pointer to given parent node.
     *
     * @see AstNode::AstNode(Token * token)
     * @see AstNode::AstNode(const AstNode & other)
     */
    AstNode(Token &token, AstNode *parent)
    : m_token(token)
    , m_parent(parent)
    {
    }

    /*!
     * @brief This function is copy constructor of AstNode class.
     *
     * This function copy data from given node to this object.
     *
     * @param[in] other Given node.
     *
     * @see AstNode::AstNode(Token * token)
     * @see AstNode::AstNode(Token * token, AstNode * parent)
     */
    AstNode(const AstNode &other);

    /*!
     * @brief Assignment operator.
     *
     * This function copy data from one to second node, when assignment
     * operator is used.
     *
     * @param[in] other Other is AstNode, from which are copied data to
     *              second AstNode.
     *
     * @return Second AstNode object.
     */
    AstNode &operator=(const AstNode &other);

    /*!
     * @brief This function is destructor of AstNode class.
     *
     * This function also call for children destructors.
     */
    virtual ~AstNode();

    /*!
     * @brief This function returns an exact duplicate of this object.
     *
     * @return Pointer to new (cloned) AstNode object.
     */
    virtual AstNode *clone() const { return new AstNode(*this); }
    /*!
     * @brief This function returns name of this node.
     *
     * @return Node name as string representation.
     */
    virtual std::string nodeName() const { return typeid(*this).name(); }
    //! @name Token
    //@{
    /*!
     * @brief This function returns token of this node.
     *
     * @return Node token.
     *
     * @see const Token * getToken()
     */
    Token &getToken() { return m_token; }
    /*!
     * @brief This function returns constant token of this node.
     *
     * @return Node constant token pointer.
     *
     * @see Token * getToken()
     */
    const Token &getToken() const { return m_token; }
    /*!
     * @brief This function set token to this node.
     *
     * @param[in] token Pointer to given token.
     */
    void setToken(Token &token) { m_token = token; }
    /*!
     * @brief This function returns value of token of this node.
     *
     * @return Node token value pointer.
     */
    Value *getTokenValue() { return m_token.getValue(); }
    /*!
     * @brief This function returns string representation of this node token value.
     *
     * @return Node token value string representation.
     */
    std::string getTokenString() { return m_token.getValue() ? m_token.getValue()->toString() : ""; }
    //@}

    //! @name Parent
    //@{
    /*!
     * @brief This function returns pointer to parent node.
     *
     * @return Pointer to parent node.
     */
    AstNode *getParent() const { return m_parent; }
    /*!
     * @brief This function set parent for this node.
     *
     * @param[in] newParent Pointer to given parent node.
     */
    void setParent(AstNode *newParent) { m_parent = newParent; }
    //@}

    //! @name Attribute
    //@{
    /*!
     * @brief This function returns count of node attributes.
     *
     * @return Size of node attributes.
     */
    size_t attributeCount() const { return m_attributes.size(); }
    /*!
     * @brief This function find attribute in AstNode attributes.
     *
     * This function find attribute from giving name in AstNode attributes.
     *
     * @param[in] name Name for AstNode attribute.
     *
     * @return True if attribute was found.
     *
     * @see Value* AstNode::getAttribute()
     * @see void AstNode::setAttribute()
     * @see void AstNode::removeAttribute()
     */
    bool hasAttribute(const std::string &name) const;

    /*!
     * @brief This function return attribute from AstNode attributes.
     *
     * This function return attribute from giving name in AstNode attributes.
     *
     * @param[in] name Name for AstNode attribute.
     *
     * @return
     *
     * @see bool AstNode::hasAttribute() const
     * @see void AstNode::setAttribute()
     * @see void AstNode::removeAttribute()
     */
    Value *getAttribute(const std::string &name);

    /*!
     * @brief This function set attribute in AstNode attributes.
     *
     * This function set attribute for giving name and node in AstNode attributes.
     *
     * @param[in] name Name for AstNode attribute.
     * @param[in] node AstNode attribute.
     *
     * @see bool AstNode::hasAttribute() const
     * @see Value* AstNode::getAttribute()
     * @see void AstNode::removeAttribute()
     */
    void setAttribute(const std::string &name, Value *node);

    /*!
     * @brief This function remove attribute from AstNode attributes.
     *
     * This function remove attribute from giving name in AstNode attributes.
     *
     * @param[in] name Name for AstNode attribute.
     *
     * @see bool AstNode::hasAttribute() const
     * @see Value* AstNode::getAttribute()
     * @see void AstNode::setAttribute()
     */
    void removeAttribute(const std::string &name);

    /*!
     * @brief Square brackets.
     *
     * This function return node attribute from attributes. Attribute is requested by
     * given attribute name in square brackets.
     *
     * @param[in] name Name of searched attribute.
     *
     * @return Node attribute.
     */
    Value *&operator[](const std::string &name) { return m_attributes[name]; }
    //@}

    //! @name Children
    //@{
    /*!
     * @brief This function add given node to the end of children list for current AstNode.
     *
     * If node is NULL then a NULL pointer is added to the children list.
     * The list node's location is automatically updated after the node is added by
     * a call to updateLocation().
     *
     * @param[in] node Child node for current AstNode.
     */
    void appendChild(AstNode *node);

    /*!
     * @brief This function return count of children for current node.
     *
     * @return Count of children for current node.
     */
    size_t childCount() const { return m_children.size(); }
    /*!
     * @brief This function return child node from node children.
     *
     * @param[in] index Index of node child.
     *
     * @return Pointer to searched child node.
     *
     * @see size_t AstNode::getIndex()
     * @see size_t getIndexOfChild()
     */

    AstNode *getChild(int index) const { return m_children[index]; }
    /*!
     * @brief This function return index of searched child node.
     *
     * @param[in] child Searched child node for current AstNode.
     *
     * @return Index of searched child node.
     * @retval -1 If child was not found.
     *
     * @see size_t AstNode::getIndex()
     * @see AstNode * getChild()
     */
    size_t getIndexOfChild(AstNode *child);

    /*!
     * @brief This function return index of current node in parent's list of children.
     *
     * @return Return value is from parent getIndexOfChild.
     *
     * @see size_t AstNode::getIndexOfChild()
     * @see AstNode * getChild()
     */
    size_t getIndex();

    /*!
     * @brief This function replace child in AstNode tree.
     *
     * This function replace original AstNode child with replacement.
     *
     * @param[in] original Original AstNode child.
     * @param[in] replacement Replacement AstNode child.
     *
     * @see void AstNode::reverseExpr()
     */
    void replaceChild(AstNode *original, AstNode *replacement);

    /*!
     * @brief This function replace children location in AstNode tree.
     *
     * This function move top of array AstNode node in AstNode tree to bottom of tree.
     *
     * @see void AstNode::replaceChild()
     */
    void reverseExpr();

    /*!
     * @brief Square brackets.
     *
     * This function return node from this node children list. Attribute is requested by
     * given attribute index number in square brackets.
     *
     * @param[in] index Index of searched child node.
     *
     * @return Node child.
     *
     * @see const AstNode *& operator [] ()
     */
    AstNode *&operator[](int index) { return m_children[index]; }
    /*!
     * @brief Square brackets.
     *
     * This function return const node from this node children list. Attribute is requested by
     * given attribute index number in square brackets.
     *
     * @param[in] index Index of searched child node.
     *
     * @return Node child const node.
     *
     * @see AstNode *& operator [] ()
     */
    const AstNode *operator[](int index) const { return m_children[index]; }
    //@}

    //! @name Child iterators
    //@{
    inline iterator begin() { return m_children.begin(); }
    inline iterator end() { return m_children.end(); }
    inline const_iterator begin() const { return m_children.begin(); }
    inline const_iterator end() const { return m_children.end(); }
    //@}

    /*!
     * @brief This function returns string description of the node.
     *
     * @return String description of the node.
     */
    std::string getDescription() const;

protected:
    Token m_token;                /*!< The token associated with this node. */
    AstNode *m_parent;            /*!< Pointer to parent node of this object. May be NULL. */
    attribute_map_t m_attributes; /*!< Map of named attributes. */
    child_list_t m_children;      /*!< Ordered list of children of this node. */
};

/*!
 * @brief Dumps an AST tree.
 */
class AstPrinter
{
public:
    /*!
     * @brief This function is constructor of AstPrinter class.
     *
     * This function set root node to given node and depth to 0.
     *
     * @param[in] root Pointer to root node.
     */
    AstPrinter(AstNode *root)
    : m_root(root)
    , m_depth(0)
    {
    }

    /*!
     * @brief This function call for dispatch function.
     *
     * This function call for dispatch with parameters root node and 0 number.
     *
     * @see void dispatch(AstNode * node, int childIndex=0)
     */
    void dispatch() { dispatch(m_root, 0); }
    /*!
     * @brief This function call for print AstNode tree information and dispatch children.
     *
     * @param[in] node Node to print.
     * @param[in] childIndex Child node index.
     *
     * @see void dispatch()
     * @see void print()
     */
    void dispatch(AstNode *node, int childIndex = 0);

    /*!
     * @brief This function print AstNode information.
     *
     * @param[in] node Node to print.
     * @param[in] childIndex Child node index.
     */
    void print(AstNode *node, int childIndex = 0);

protected:
    AstNode *m_root;              /*!< Root of AstNode tree. */
    int m_depth;                  /*!< Depth of AstNode tree. */
    std::stack<int> m_depthStack; /*!< Vector of depth. */

    /*!
     * @brief This function call for print indent.
     *
     * @param[in] indent Number of spaces.
     */
    void printIndent(int indent) const;
};

} // namespace erpcgen

#endif // _EMBEDDED_RPC__ASTNODE_H_
