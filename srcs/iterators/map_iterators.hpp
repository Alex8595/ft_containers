/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_iterators.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahernand <ahernand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/10 15:19:14 by ahernand          #+#    #+#             */
/*   Updated: 2022/05/14 19:23:44 by ahernand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MAP_ITERATORS__
# define __MAP_ITERATORS__
# include "BST.hpp"
# include <iostream>

namespace ft
{
	template <class paired, class node_type>
	class map_iterator
	{
		public:
			typedef node_type* 								pointer;
			typedef node_type&								reference;

			node_type										*_ptr;

		private:

		public:

			//
			//		_______________________________ Operators _______________________________
			//




			//		________		>< Operators

			template <typename K, typename N>
			bool					operator!=(const map_iterator<K, N> &ref) const
			{
				if (this->_ptr != ref._ptr)
				{
					return (true);
				}
				return (false);
			}




			////////		________		++-- Operators

			map_iterator					&operator++()
			{
				_ptr = bst_increment(_ptr);
				return (*this);
			}

			map_iterator					operator++(int)
			{
				map_iterator it_aux = *this;
				++(*this);
				return (it_aux);
			}




			//		________				Copy Operators


			template <typename K, typename N>
			map_iterator					&operator=(const map_iterator<K, N> &ref)
			{
				this->_ptr = ref._ptr;
				//std::cout << "aqui 3" << std::endl;
				return (*this);
			}

			reference						operator*()
			{
				return (*_ptr);
			}

			paired*							operator->()
			{
				return (&_ptr->data);
			}




			//
			//		______________________________ Cons & Dest ______________________________
			//

			template <typename K, typename N>
			map_iterator(map_iterator<K, N> a)
			{
				_ptr = a._ptr;
			}

			map_iterator()
			{
				_ptr = NULL;
			}

			map_iterator(pointer	 ptr)
			{
				_ptr = ptr;
			}

			~map_iterator()
			{
			}
	};
}

#endif