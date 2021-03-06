/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahernand <ahernand@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/03 19:55:43 by ahernand          #+#    #+#             */
/*   Updated: 2022/05/27 14:22:55 by ahernand         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef __MAP_HPP__
# define __MAP_HPP__
# include "iterators/functions.hpp"
# include "iterators/map_iterators.hpp"

namespace ft
{
	template <class Key, class T, class Compare = std::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
		public:
			typedef Key																key_type;
			typedef T																mapped_type;
			typedef ft::pair<const Key, T>											value_type;
			typedef	Compare															key_compare;
			typedef Alloc															allocator_type;
			typedef value_type&														reference;
			typedef const value_type&												const_reference;
			typedef value_type*														pointer;
			typedef const value_type*												const_pointer;
			typedef node<value_type>												node_type;
			typedef ft::map_iterator<value_type, node_type, Compare>				iterator;
			typedef ft::map_iterator<const value_type, node_type, Compare>			const_iterator;
			typedef ft::reverse_map_iterator<iterator>								reverse_iterator;
			typedef ft::reverse_map_iterator<const_iterator>						const_reverse_iterator;
			typedef ptrdiff_t														difference_type;
			typedef size_t															size_type;

		private:
			node<value_type>														*_root;
			typedef typename Alloc::template rebind<node<value_type> >::other		node_allocator;
			
			allocator_type															_allocator;
			node_allocator															_n_allocator;
			Compare																	_compare;
			size_type																_size;

		public:



			/*
			**		___________________		Iterators     ___________________
			*/

			iterator						begin()
			{
				return (bst_get_first(_root));
			}

			const_iterator					begin() const
			{
				return (bst_get_first(_root));
			}

			iterator						end()
			{
				return (bst_get_last(_root));
			}

			const_iterator					end() const
			{
				return (bst_get_last(_root));
			}




			reverse_iterator				rbegin()
			{
				return (ft::reverse_map_iterator<iterator>(bst_get_last(_root)));
			}

			const_reverse_iterator			rbegin() const
			{
				return (ft::reverse_map_iterator<const iterator>(bst_get_last(_root)));
			}

			reverse_iterator				rend()
			{
				return (ft::reverse_map_iterator<iterator>(bst_get_first(_root)));
			}

			const_reverse_iterator			rend() const
			{
				return (ft::reverse_map_iterator<const iterator>(bst_get_first(_root)));
			}








			/*
			**		___________________		Capacity     ___________________
			*/




			bool							empty() const
			{
				if (_size == 0)
					return (true);
				return (false);
			}




			size_type						size() const
			{
				return (_size);				
			}




			size_type						max_size() const
			{
				return (std::numeric_limits<difference_type>::max() / (sizeof(node_type) / 2 ?: 1));
			}








			/*
			**		___________________		Element Access     ___________________
			*/

			mapped_type&					operator[] (const key_type& k)
			{
				node_type					*aux;

				aux = bst_search(_root, k, _compare);
				if (aux == NULL)
				{
					T		b;

					insert(ft::pair<const Key, T>(k, b));
					aux = bst_search(_root, k, _compare);
				}
				return (aux->data.second);
			}








			/*
			**		___________________		Modifiers     ___________________
			*/




			//		___________________		Insert     ___________________	




			ft::pair<iterator, bool>		insert (const value_type &val)
			{
				bool						existed;

				if (bst_search(_root, val.first, _compare) == NULL)
				{
					existed = true;
				}
				else
					existed = false;

				if (_size == 0)
				{
					node<value_type>		*aux;
					node<value_type>        c_aux(val, NULL, 0);
					
					aux = _root;
					_root = _n_allocator.allocate(1);
					_n_allocator.construct(_root, c_aux);
					_root->right = aux;
					_root->right->parent = _root;
				}
				else
				{
					create_node(_root, _root, val, _compare, _n_allocator);
				}

				if (existed == true)
					_size++;
				return (ft::pair<iterator, bool>(iterator(bst_search(_root, val.first, _compare)), existed));
			}




			iterator						insert(iterator position, const value_type& val)
			{
				(void)position;
				insert(val);
				return (iterator(bst_search(_root, val.first, _compare)));
			}




			template <class InputIterator>
			void							insert (InputIterator first, InputIterator last)
			{

				for (; first != last; first++)
				{
					insert(ft::pair<const Key, T>((first)->first, (first)->second));
				}
			}








			//		___________________		Erease     ___________________	




			size_type						erase(const key_type &val)
			{
				mapped_type				mapped_aux;

				if (bst_search(_root, val, _compare) == NULL)
					return (0);				
				_root = destroy_node(_root, ft::pair<const key_type, mapped_type>(val, mapped_aux), _root, _compare, _n_allocator);
				--_size;
				return (1);
			}




			void							erase(iterator position)
			{
				erase (position->first);
			}




			void							erase(iterator first, iterator last)
			{
				while (first != last)
				{
					erase (first);
					first++;
				}
			}








			//		___________________		Swap     ___________________	




			void	swap (map& x)
			{
				ft::map<Key, T, Compare, Alloc>						aux;
				_n_allocator.deallocate(aux._root, 1);
				aux.complete_memory_copy(*this);
				this->complete_memory_copy(x);
				x.complete_memory_copy(aux);
				aux._root = NULL;
			}








			//		___________________		Swap     ___________________	




			void clear()
			{
				Key							a;
				T							b;
				node<value_type>      	  c_aux(ft::pair<const Key, T>(a, b), NULL, 1);

				_size = 0;
				clear_tree(_root, _n_allocator);
				_root = _n_allocator.allocate(1);
				_n_allocator.construct(_root, c_aux);
			}



					




			/*
			**		___________________		Observers     ___________________
			*/


			class													value_compare
			{
				friend class										map;
					protected:
						Compare										comp;
						value_compare (Compare c) : comp(c)
						{

						}

					public:
						typedef bool 								result_type;
						typedef value_type							first_argument_type;
						typedef value_type							second_argument_type;

						bool operator() (const value_type& x, const value_type& y) const
						{
							return comp(x.first, y.first);
						}

						bool operator() (const value_type* x, const value_type* y) const
						{
							return comp(x->first, y->first);
						}
			};




			key_compare						key_comp() const
			{
				return (_compare);
			}




			value_compare					value_comp() const
			{
				return (value_compare(_compare));
			}








			/*
			**		___________________		Operations     ___________________
			*/




			iterator						find(const key_type &k)
			{
				node_type					*aux;

				aux = bst_search(_root, k, _compare);
				if (aux == NULL)
					return (end());
				return (iterator(aux));
			}




			const_iterator						find(const key_type &k) const
			{
				node_type						*aux;

				aux = bst_search(_root, k, _compare);
				if (aux == NULL)
					return (end());
				return (const_iterator(aux));
			}




			size_type count (const key_type& k) const
			{
				return (bst_search(_root, k, _compare) != NULL);
			}




			iterator							lower_bound (const key_type& k)
			{
				typename ft::map<Key, T>::iterator					it = begin();
				typename ft::map<Key, T>::iterator					ite = end();

				for (; it != ite; ++it)
				{
					if (it->first >= k)
						return (it);
				}
				return (ite);
			}




			const_iterator						lower_bound (const key_type& k) const
			{
				typename ft::map<Key, T>::const_iterator			it = begin();
				typename ft::map<Key, T>::const_iterator			ite = end();
				
				for (; it != ite; ++it)
				{
					if (it->first >= k)
						return (it);
				}
				return (ite);
			}




			iterator								upper_bound (const key_type& k)
			{
				typename ft::map<Key, T>::iterator					it = begin();
				typename ft::map<Key, T>::iterator					ite = end();

				for (; it != ite; ++it)
				{
					if (it->first > k)
						return (it);
				}
				return (ite);
			}




			const_iterator							upper_bound (const key_type& k) const
			{
				typename ft::map<Key, T>::const_iterator			it = begin();
				typename ft::map<Key, T>::const_iterator			ite = end();

				for (; it != ite; ++it)
				{
					if (it->first > k)
						return (it);
				}
				return (ite);
			}




			pair<iterator, iterator>					equal_range (const key_type& k)
			{
				typename ft::map<Key, T>::iterator					it1 = lower_bound(k);
				typename ft::map<Key, T>::iterator					it2 = upper_bound(k);
				
				return (pair<iterator,iterator>(it1, it2));
			}




			pair<const_iterator, const_iterator>		equal_range (const key_type& k) const
			{
				typename ft::map<Key, T>::const_iterator			it1 = lower_bound(k);
				typename ft::map<Key, T>::const_iterator			it2 = upper_bound(k);
				
				return (pair<const_iterator,const_iterator>(it1, it2));
			}








			/*
			**		___________________		Allocator     ___________________
			*/




			allocator_type					get_allocator() const
			{
				return (_allocator);				
			}








			/*
			**		___________________		Operator =     ___________________
			*/

			map&							operator= (const map& x)
			{
				clear();
				insert(x.begin(), x.end());
				return (*this);
			}








			/*
			**		___________________		Cons & Dest     ___________________
			*/




			explicit map (const key_compare& comp = key_compare(), const allocator_type& alloc = allocator_type())
			{
				Key						a;
				T						b;
				node<value_type>		aux(ft::pair<const Key, T>(a, b), NULL, 1);

				_size = 0;
				_compare = comp;
				_allocator = alloc;

				_root = _n_allocator.allocate(1);
				_n_allocator.construct(_root, aux);
			}




			template <class InputIterator>
			map (InputIterator first, InputIterator last,
				const key_compare& comp = key_compare(),
				const allocator_type& alloc = allocator_type())
			{
				Key						a;
				T						b;
				node<value_type>		aux(ft::pair<const Key, T>(a, b), NULL, 1);


				_size = 0;
				_compare = comp;
				_allocator = alloc;

				_root = _n_allocator.allocate(1);
				_n_allocator.construct(_root, aux);
				
				insert(first, last);
			}




			map (const map& x)
			{
				Key						a;
				T						b;
				node<value_type>		aux(ft::pair<const Key, T>(a, b), NULL, 1);
				
				_size = 0;
				
				_root = _n_allocator.allocate(1);
				_n_allocator.construct(_root, aux);

				insert(x.begin(), x.end());
			}




			~map()
			{
				clear_tree(_root, _n_allocator);
			}








			/*
			**		___________________		Swap aux function  ___________________
			*/




			private:

				void	complete_memory_copy(map	&x)
				{
					_size = x.size();
					_allocator = x.get_allocator();
					_compare = x._compare; 
					_root = x._root;
				}
	};








	/*
	**		___________________		External Operators     ___________________
	*/




	template< class Key, class T, class Compare, class Alloc >
		bool operator==(	const ft::map<Key, T, Compare, Alloc >& lhs,
							const ft::map<Key, T, Compare, Alloc >& rhs )
	{
		if (lhs.size() != rhs.size())
			return (false);

		typename ft::map<Key, T, Compare, Alloc>::const_iterator		l_it = lhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		l_ite = lhs.end();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		r_it = rhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		r_ite = rhs.end();

		while (l_it != l_ite)
		{
			if (l_it->first != r_it->first || l_it->second != r_it->second)
				return (false);
			l_it++;
			r_it++;
		}
		return (true);
	}




	template< class Key, class T, class Compare, class Alloc >
		bool operator!=(	const ft::map<Key, T, Compare, Alloc >& lhs,
							const ft::map<Key, T, Compare, Alloc >& rhs )
	{
		return (!(lhs == rhs));
	}




	template< class Key, class T, class Compare, class Alloc >
		bool operator<(	const ft::map<Key, T, Compare, Alloc >& lhs,
							const ft::map<Key, T, Compare, Alloc >& rhs )
	{
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		first1 = lhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		last1 = lhs.end();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		first2 = rhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		last2 = rhs.end();

		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (first1->second < first2->second)
				return (true);
			if (first1->second > first2->second)
				return (false);
		}
		if (lhs.size() < rhs.size())
			return (true);
		else if (lhs.size() > rhs.size() )
			return (false);
		return (first1 == last1) && (first2 != last2);
	}




	template< class Key, class T, class Compare, class Alloc >
		bool operator>(	const ft::map<Key, T, Compare, Alloc >& lhs,
							const ft::map<Key, T, Compare, Alloc >& rhs )
	{
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		first1 = lhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		last1 = lhs.end();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		first2 = rhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator		last2 = rhs.end();
		

		for ( ; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2)
		{
			if (first1->second > first2->second)
				return (true);
			if (first1->second < first2->second)
				return (false);
		}
		if (lhs.size() > rhs.size())
			return (true);
		else if (lhs.size() < rhs.size() )
			return (false);
		return (first1 == last1) && (first2 != last2);
	}




	template< class Key, class T, class Compare, class Alloc >
		bool operator<=(	const ft::map<Key, T, Compare, Alloc >& lhs,
							const ft::map<Key, T, Compare, Alloc >& rhs )
	{
		return ((lhs == rhs) || (lhs < rhs));
	}




	template< class Key, class T, class Compare, class Alloc >
		bool operator>=(	const ft::map<Key, T, Compare, Alloc >& lhs,
							const ft::map<Key, T, Compare, Alloc >& rhs )
	{
		return ((lhs == rhs) || (lhs > rhs));
	}

}

#endif