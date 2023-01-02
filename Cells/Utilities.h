#pragma once
#include <vector>
#include "Compound.h"
#include "Element.h"

template <class T>
void FastDelete(std::vector<T> &vec, int idx);

template <class T>
void FastDelete(std::vector<T> &vec, int idx)
{
	vec[idx] = vec.back();
	vec.pop_back();
}

template <class T, class U>
class ConnectionNode
{
public:
	T parent;
	std::vector<ConnectionNode<U, T>*> cons;
	std::vector<size_t> meta;
	std::vector<U> vec;
};



template <class T, class U>
void FastConnect(ConnectionNode<T, U>* tnode, ConnectionNode<U, T>* unode)
{
	tnode->meta.emplace_back(unode->cons.size());
	unode->meta.emplace_back(tnode->cons.size());
	tnode->cons.emplace_back(unode);
	unode->cons.emplace_back(tnode);
	tnode->vec.emplace_back(unode->parent);
	unode->vec.emplace_back(tnode->parent);
}

template <class T, class U>
void FastDisconnect(ConnectionNode<T, U>* tnode, int tindex)
{
	int uindex = tnode->meta[tindex];
	ConnectionNode<U, T>* unode = tnode->cons[tindex];

	FastDelete(tnode->cons, tindex);
	FastDelete(tnode->meta, tindex);
	FastDelete(tnode->vec, tindex);

	//if the size is equal to tindex, then we just fastdeleted the last thing in the vector and nothing needs to move
	if (tnode->cons.size() != tindex)
	{
		//the node we just moved in our connection list
		ConnectionNode<U, T>* movedConnection = tnode->cons[tindex];
		//the metadata attached to that node
		size_t metadataOfMovedConnection = tnode->meta[tindex];
		//tell that node that we moved it, and change its metadata accordingly
		movedConnection->meta[metadataOfMovedConnection] = tindex;
		//now we are still connected to it, and it is connected to us,
		//but it knows that we moved it in our list,
		//and we havent moved in it's list so we didn't need to change the metadata we have about it
	}

	//repeat all that for unode
	FastDelete(unode->cons, uindex);
	FastDelete(unode->meta, uindex);
	FastDelete(unode->vec, uindex);


	//if the size is equal to tindex, then we just fastdeleted the last thing in the vector and nothing needs to move
	if (unode->cons.size() != uindex)
	{
		//the node we just moved in our connection list
		ConnectionNode<T, U>* movedConnection = unode->cons[uindex];
		//the metadata attached to that node
		size_t metadataOfMovedConnection = unode->meta[uindex];
		//tell that node that we moved it, and change its metadata accordingly
		movedConnection->meta[metadataOfMovedConnection] = uindex;
		//now we are still connected to it, and it is connected to us,
		//but it knows that we moved it in our list,
		//and we havent moved in it's list so we didn't need to change the metadata we have about it
	}
}