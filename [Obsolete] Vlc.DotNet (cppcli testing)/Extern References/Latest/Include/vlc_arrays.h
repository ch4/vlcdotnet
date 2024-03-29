/*****************************************************************************
 * vlc_arrays.h : Arrays and data structures handling
 *****************************************************************************
 * Copyright (C) 1999-2004 the VideoLAN team
 * $Id: 390c1b6d9df4d275619b9c62945d352e063b4229 $
 *
 * Authors: Samuel Hocevar <sam@zoy.org>
 *          Clément Stenac <zorglub@videolan.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston MA 02110-1301, USA.
 *****************************************************************************/

#ifndef VLC_ARRAYS_H_
#define VLC_ARRAYS_H_

/**
 * \file
 * This file defines functions, structures and macros for handling arrays in vlc
 */

/* realloc() that never fails *if* downsizing */
static inline void *realloc_down( void *ptr, size_t size )
{
    void *ret = realloc( ptr, size );
    return ret ? ret : ptr;
}

/**
 * Simple dynamic array handling. Array is realloced at each insert/removal
 */
#if defined( _MSC_VER ) && _MSC_VER < 1300 && !defined( UNDER_CE )
#   define VLCCVP (void**) /* Work-around for broken compiler */
#else
#   define VLCCVP
#endif
#define INSERT_ELEM( p_ar, i_oldsize, i_pos, elem )                           \
    do                                                                        \
    {                                                                         \
        if( !i_oldsize ) (p_ar) = NULL;                                       \
        (p_ar) = VLCCVP realloc( p_ar, ((i_oldsize) + 1) * sizeof(*(p_ar)) ); \
        if( !(p_ar) ) abort();                                                \
        if( (i_oldsize) - (i_pos) )                                           \
        {                                                                     \
            memmove( (p_ar) + (i_pos) + 1, (p_ar) + (i_pos),                  \
                     ((i_oldsize) - (i_pos)) * sizeof( *(p_ar) ) );           \
        }                                                                     \
        (p_ar)[i_pos] = elem;                                                 \
        (i_oldsize)++;                                                        \
    }                                                                         \
    while( 0 )

#define REMOVE_ELEM( p_ar, i_size, i_pos )                                    \
    do                                                                        \
    {                                                                         \
        if( (i_size) - (i_pos) - 1 )                                          \
        {                                                                     \
            memmove( (p_ar) + (i_pos),                                        \
                     (p_ar) + (i_pos) + 1,                                    \
                     ((i_size) - (i_pos) - 1) * sizeof( *(p_ar) ) );          \
        }                                                                     \
        if( i_size > 1 )                                                      \
            (p_ar) = realloc_down( p_ar, ((i_size) - 1) * sizeof( *(p_ar) ) );\
        else                                                                  \
        {                                                                     \
            free( p_ar );                                                     \
            (p_ar) = NULL;                                                    \
        }                                                                     \
        (i_size)--;                                                           \
    }                                                                         \
    while( 0 )

#define TAB_INIT( count, tab )                  \
  do {                                          \
    (count) = 0;                                \
    (tab) = NULL;                               \
  } while(0)

#define TAB_CLEAN( count, tab )                 \
  do {                                          \
    free( tab );                                \
    (count)= 0;                                 \
    (tab)= NULL;                                \
  } while(0)

#define TAB_APPEND_CAST( cast, count, tab, p )             \
  do {                                          \
    if( (count) > 0 )                           \
        (tab) = cast realloc( tab, sizeof( void ** ) * ( (count) + 1 ) ); \
    else                                        \
        (tab) = cast malloc( sizeof( void ** ) );    \
    if( !(tab) ) abort();                       \
    (tab)[count] = (p);                         \
    (count)++;                                  \
  } while(0)

#define TAB_APPEND( count, tab, p )             \
    TAB_APPEND_CAST( , count, tab, p )
#define TAB_APPEND_CPP( type, count, tab, p )   \
    TAB_APPEND_CAST( (type**), count, tab, p )

#define TAB_FIND( count, tab, p, index )        \
  do {                                          \
        int _i_;                                \
        (index) = -1;                           \
        for( _i_ = 0; _i_ < (count); _i_++ )    \
        {                                       \
            if( (tab)[_i_] == (p) )             \
            {                                   \
                (index) = _i_;                  \
                break;                          \
            }                                   \
        }                                       \
  } while(0)


#define TAB_REMOVE( count, tab, p )             \
  do {                                          \
        int _i_index_;                          \
        TAB_FIND( count, tab, p, _i_index_ );   \
        if( _i_index_ >= 0 )                    \
        {                                       \
            if( (count) > 1 )                   \
            {                                   \
                memmove( ((void**)(tab) + _i_index_),    \
                         ((void**)(tab) + _i_index_+1),  \
                         ( (count) - _i_index_ - 1 ) * sizeof( void* ) );\
            }                                   \
            (count)--;                          \
            if( (count) == 0 )                  \
            {                                   \
                free( tab );                    \
                (tab) = NULL;                   \
            }                                   \
        }                                       \
  } while(0)

#define TAB_INSERT_CAST( cast, count, tab, p, index ) do { \
    if( (count) > 0 )                           \
        (tab) = cast realloc( tab, sizeof( void ** ) * ( (count) + 1 ) ); \
    else                                        \
        (tab) = cast malloc( sizeof( void ** ) );       \
    if( !(tab) ) abort();                       \
    if( (count) - (index) > 0 )                 \
        memmove( (void**)(tab) + (index) + 1,   \
                 (void**)(tab) + (index),       \
                 ((count) - (index)) * sizeof(*(tab)) );\
    (tab)[(index)] = (p);                       \
    (count)++;                                  \
} while(0)

#define TAB_INSERT( count, tab, p, index )      \
    TAB_INSERT_CAST( , count, tab, p, index )

/**
 * Binary search in a sorted array. The key must be comparable by < and >
 * \param entries array of entries
 * \param count number of entries
 * \param elem key to check within an entry (like .id, or ->i_id)
 * \param zetype type of the key
 * \param key value of the key
 * \param answer index of answer within the array. -1 if not found
 */
#define BSEARCH( entries, count, elem, zetype, key, answer ) \
   do {  \
    int low = 0, high = count - 1;   \
    answer = -1; \
    while( low <= high ) {\
        int mid = (low + high ) / 2; /* Just don't care about 2^30 tables */ \
        zetype mid_val = entries[mid] elem;\
        if( mid_val < key ) \
            low = mid + 1; \
        else if ( mid_val > key ) \
            high = mid -1;  \
        else    \
        {   \
            answer = mid;  break;   \
        }\
    } \
 } while(0)


/************************************************************************
 * Dynamic arrays with progressive allocation
 ************************************************************************/

/* Internal functions */
#define _ARRAY_ALLOC(array, newsize) {                                      \
    (array).i_alloc = newsize;                                              \
    (array).p_elems = VLCCVP realloc( (array).p_elems, (array).i_alloc *    \
                                    sizeof(*(array).p_elems) );             \
    if( !(array).p_elems ) abort();                                         \
}

#define _ARRAY_GROW1(array) {                                               \
    if( (array).i_alloc < 10 )                                              \
        _ARRAY_ALLOC(array, 10 )                                            \
    else if( (array).i_alloc == (array).i_size )                            \
        _ARRAY_ALLOC(array, (int)(array.i_alloc * 1.5) )                    \
}

#define _ARRAY_GROW(array,additional) {                                     \
     int i_first = (array).i_alloc;                                         \
     while( (array).i_alloc - i_first < additional )                        \
     {                                                                      \
         if( (array).i_alloc < 10 )                                         \
            _ARRAY_ALLOC(array, 10 )                                        \
        else if( (array).i_alloc == (array).i_size )                        \
            _ARRAY_ALLOC(array, (int)((array).i_alloc * 1.5) )              \
        else break;                                                         \
     }                                                                      \
}

#define _ARRAY_SHRINK(array) {                                              \
    if( (array).i_size > 10 && (array).i_size < (int)((array).i_alloc / 1.5) ) {  \
        _ARRAY_ALLOC(array, (array).i_size + 5);                            \
    }                                                                       \
}

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/* API */
#define DECL_ARRAY(type) struct {                                           \
    int i_alloc;                                                            \
    int i_size;                                                             \
    type *p_elems;                                                          \
}

#define TYPEDEF_ARRAY(type, name) typedef DECL_ARRAY(type) name;

#define ARRAY_INIT(array)                                                   \
  do {                                                                      \
    (array).i_alloc = 0;                                                    \
    (array).i_size = 0;                                                     \
    (array).p_elems = NULL;                                                 \
  } while(0)

#define ARRAY_RESET(array)                                                  \
  do {                                                                      \
    (array).i_alloc = 0;                                                    \
    (array).i_size = 0;                                                     \
    free( (array).p_elems ); (array).p_elems = NULL;                        \
  } while(0)

#define ARRAY_APPEND(array, elem)                                           \
  do {                                                                      \
    _ARRAY_GROW1(array);                                                    \
    (array).p_elems[(array).i_size] = elem;                                 \
    (array).i_size++;                                                       \
  } while(0)

#define ARRAY_INSERT(array,elem,pos)                                        \
  do {                                                                      \
    _ARRAY_GROW1(array);                                                    \
    if( (array).i_size - pos ) {                                            \
        memmove( (array).p_elems + pos + 1, (array).p_elems + pos,          \
                 ((array).i_size-pos) * sizeof(*(array).p_elems) );         \
    }                                                                       \
    (array).p_elems[pos] = elem;                                            \
    (array).i_size++;                                                       \
  } while(0)

#define ARRAY_REMOVE(array,pos)                                             \
  do {                                                                      \
    if( (array).i_size - (pos) - 1 )                                        \
    {                                                                       \
        memmove( (array).p_elems + pos, (array).p_elems + pos + 1,          \
                 ( (array).i_size - pos - 1 ) *sizeof(*(array).p_elems) );  \
    }                                                                       \
    (array).i_size--;                                                       \
    _ARRAY_SHRINK(array);                                                   \
  } while(0)

#define ARRAY_VAL(array, pos) array.p_elems[pos]

#define ARRAY_BSEARCH(array, elem, zetype, key, answer) \
    BSEARCH( (array).p_elems, (array).i_size, elem, zetype, key, answer)

#define FOREACH_ARRAY( item, array ) { \
    int fe_idx; \
    for( fe_idx = 0 ; fe_idx < (array).i_size ; fe_idx++ ) \
    { \
        item = (array).p_elems[fe_idx];

#define FOREACH_END() } }


/************************************************************************
 * Dynamic arrays with progressive allocation (Preferred API)
 ************************************************************************/
typedef struct vlc_array_t
{
    int i_count;
    void ** pp_elems;
} vlc_array_t;

static inline void vlc_array_init( vlc_array_t * p_array )
{
    memset( p_array, 0, sizeof(vlc_array_t) );
}

static inline void vlc_array_clear( vlc_array_t * p_array )
{
    free( p_array->pp_elems );
    memset( p_array, 0, sizeof(vlc_array_t) );
}

static inline vlc_array_t * vlc_array_new( void )
{
    vlc_array_t * ret = (vlc_array_t *)malloc( sizeof(vlc_array_t) );
    if( ret ) vlc_array_init( ret );
    return ret;
}

static inline void vlc_array_destroy( vlc_array_t * p_array )
{
    if( !p_array )
        return;
    vlc_array_clear( p_array );
    free( p_array );
}


/* Read */
static inline int
vlc_array_count( vlc_array_t * p_array )
{
    return p_array->i_count;
}

static inline void *
vlc_array_item_at_index( vlc_array_t * p_array, int i_index )
{
    return p_array->pp_elems[i_index];
}

static inline int
vlc_array_index_of_item( vlc_array_t * p_array, void * item )
{
    int i;
    for( i = 0; i < p_array->i_count; i++)
    {
        if( p_array->pp_elems[i] == item )
            return i;
    }
    return -1;
}

/* Write */
static inline void
vlc_array_insert( vlc_array_t * p_array, void * p_elem, int i_index )
{
    TAB_INSERT_CAST( (void **), p_array->i_count, p_array->pp_elems, p_elem, i_index );
}

static inline void
vlc_array_append( vlc_array_t * p_array, void * p_elem )
{
    vlc_array_insert( p_array, p_elem, p_array->i_count );
}

static inline void
vlc_array_remove( vlc_array_t * p_array, int i_index )
{
    if( i_index >= 0 )
    {
        if( p_array->i_count > 1 )
        {
            memmove( p_array->pp_elems + i_index,
                     p_array->pp_elems + i_index+1,
                     ( p_array->i_count - i_index - 1 ) * sizeof( void* ) );
        }
        p_array->i_count--;
        if( p_array->i_count == 0 )
        {
            free( p_array->pp_elems );
            p_array->pp_elems = NULL;
        }
    }
}


/************************************************************************
 * Dictionaries
 ************************************************************************/

/* This function is not intended to be crypto-secure, we only want it to be
 * fast and not suck too much. This one is pretty fast and did 0 collisions
 * in wenglish's dictionary.
 */
static inline uint64_t DictHash( const char *psz_string, int hashsize )
{
    uint64_t i_hash = 0;
    if( psz_string )
    {
        while( *psz_string )
        {
            i_hash += *psz_string++;
            i_hash += i_hash << 10;
            i_hash ^= i_hash >> 8;
        }
    }
    return i_hash % hashsize;
}

typedef struct vlc_dictionary_entry_t
{
    char *   psz_key;
    void *   p_value;
    struct vlc_dictionary_entry_t * p_next;
} vlc_dictionary_entry_t;

typedef struct vlc_dictionary_t
{
    int i_size;
    vlc_dictionary_entry_t ** p_entries;
} vlc_dictionary_t;

static void * const kVLCDictionaryNotFound = NULL;

static inline void vlc_dictionary_init( vlc_dictionary_t * p_dict, int i_size )
{
    p_dict->p_entries = NULL;

    if( i_size > 0 )
    {
        p_dict->p_entries = (vlc_dictionary_entry_t **)calloc( i_size, sizeof(*p_dict->p_entries) );
        if( !p_dict->p_entries )
            i_size = 0;
    }
    p_dict->i_size = i_size;
}

static inline void vlc_dictionary_clear( vlc_dictionary_t * p_dict,
                                         void ( * pf_free )( void * p_data, void * p_obj ),
                                         void * p_obj )
{
    if( p_dict->p_entries )
    {
        for( int i = 0; i < p_dict->i_size; i++ )
        {
            vlc_dictionary_entry_t * p_current, * p_next;
            p_current = p_dict->p_entries[i];
            while( p_current )
            {
                p_next = p_current->p_next;
                if( pf_free != NULL )
                    ( * pf_free )( p_current->p_value, p_obj );
                free( p_current->psz_key );
                free( p_current );
                p_current = p_next;
            }
        }
        free( p_dict->p_entries );
        p_dict->p_entries = NULL;
    }
    p_dict->i_size = 0;
}



static inline void *
vlc_dictionary_value_for_key( const vlc_dictionary_t * p_dict, const char * psz_key )
{
    if( !p_dict->p_entries )
        return kVLCDictionaryNotFound;

    int i_pos = DictHash( psz_key, p_dict->i_size );
    vlc_dictionary_entry_t * p_entry = p_dict->p_entries[i_pos];

    if( !p_entry )
        return kVLCDictionaryNotFound;

    /* Make sure we return the right item. (Hash collision) */
    do {
        if( !strcmp( psz_key, p_entry->psz_key ) )
            return p_entry->p_value;
        p_entry = p_entry->p_next;
    } while( p_entry );

    return kVLCDictionaryNotFound;
}

static inline int
vlc_dictionary_keys_count( const vlc_dictionary_t * p_dict )
{
    vlc_dictionary_entry_t * p_entry;
    int i, count = 0;

    if( !p_dict->p_entries )
        return 0;

    for( i = 0; i < p_dict->i_size; i++ )
    {
        for( p_entry = p_dict->p_entries[i]; p_entry; p_entry = p_entry->p_next ) count++;
    }
    return count;
}

static inline char **
vlc_dictionary_all_keys( const vlc_dictionary_t * p_dict )
{
    vlc_dictionary_entry_t * p_entry;
    char ** ppsz_ret;
    int i, count = vlc_dictionary_keys_count( p_dict );

    ppsz_ret = (char**)malloc(sizeof(char *) * (count + 1));

    count = 0;
    for( i = 0; i < p_dict->i_size; i++ )
    {
        for( p_entry = p_dict->p_entries[i]; p_entry; p_entry = p_entry->p_next )
            ppsz_ret[count++] = strdup( p_entry->psz_key );
    }
    ppsz_ret[count] = NULL;
    return ppsz_ret;
}

static inline void
__vlc_dictionary_insert( vlc_dictionary_t * p_dict, const char * psz_key,
                         void * p_value, bool rebuild )
{
    if( !p_dict->p_entries )
        vlc_dictionary_init( p_dict, 1 );

    int i_pos = DictHash( psz_key, p_dict->i_size );
    vlc_dictionary_entry_t * p_entry;

    p_entry = (vlc_dictionary_entry_t *)malloc(sizeof(*p_entry));
    p_entry->psz_key = strdup( psz_key );
    p_entry->p_value = p_value;
    p_entry->p_next = p_dict->p_entries[i_pos];
    p_dict->p_entries[i_pos] = p_entry;
    if( rebuild )
    {
        /* Count how many items there was */
        int count;
        for( count = 1; p_entry->p_next; count++ )
            p_entry = p_entry->p_next;
        if( count > 3 ) /* XXX: this need tuning */
        {
            /* Here it starts to be not good, rebuild a bigger dictionary */
            struct vlc_dictionary_t new_dict;
            int i_new_size = ( (p_dict->i_size+2) * 3) / 2; /* XXX: this need tuning */
            int i;
            vlc_dictionary_init( &new_dict, i_new_size );
            for( i = 0; i < p_dict->i_size; i++ )
            {
                p_entry = p_dict->p_entries[i];
                while( p_entry )
                {
                    __vlc_dictionary_insert( &new_dict, p_entry->psz_key,
                                             p_entry->p_value,
                                             false /* To avoid multiple rebuild loop */);
                    p_entry = p_entry->p_next;
                }
            }

            vlc_dictionary_clear( p_dict, NULL, NULL );
            p_dict->i_size = new_dict.i_size;
            p_dict->p_entries = new_dict.p_entries;
        }
    }
}

static inline void
vlc_dictionary_insert( vlc_dictionary_t * p_dict, const char * psz_key, void * p_value )
{
    __vlc_dictionary_insert( p_dict, psz_key, p_value, true );
}

static inline void
vlc_dictionary_remove_value_for_key( const vlc_dictionary_t * p_dict, const char * psz_key,
                                     void ( * pf_free )( void * p_data, void * p_obj ),
                                     void * p_obj )
{
    if( !p_dict->p_entries )
        return;

    int i_pos = DictHash( psz_key, p_dict->i_size );
    vlc_dictionary_entry_t * p_entry = p_dict->p_entries[i_pos];
    vlc_dictionary_entry_t * p_prev;

    if( !p_entry )
        return; /* Not found, nothing to do */

    /* Hash collision */
    p_prev = NULL;
    do {
        if( !strcmp( psz_key, p_entry->psz_key ) )
        {
            if( pf_free != NULL )
                ( * pf_free )( p_entry->p_value, p_obj );
            if( !p_prev )
                p_dict->p_entries[i_pos] = p_entry->p_next;
            else
                p_prev->p_next = p_entry->p_next;
            free( p_entry->psz_key );
            free( p_entry );
            return;
        }
        p_prev = p_entry;
        p_entry = p_entry->p_next;
    } while( p_entry );

    /* No key was found */
}

#endif
