/*****************************************************************************
 * vlc_meta.h: Stream meta-data
 *****************************************************************************
 * Copyright (C) 2004 the VideoLAN team
 * $Id: 8481bd19bc639466fb278e9b57bb51f37951fb94 $
 *
 * Authors: Laurent Aimar <fenrir@via.ecp.fr>
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

#ifndef VLC_META_H
#define VLC_META_H 1

/**
 * \file
 * This file defines functions and structures for stream meta-data in vlc
 *
 */

typedef enum vlc_meta_type_t
{
    vlc_meta_Title,
    vlc_meta_Artist,
    vlc_meta_Genre,
    vlc_meta_Copyright,
    vlc_meta_Album,
    vlc_meta_TrackNumber,
    vlc_meta_Description,
    vlc_meta_Rating,
    vlc_meta_Date,
    vlc_meta_Setting,
    vlc_meta_URL,
    vlc_meta_Language,
    vlc_meta_NowPlaying,
    vlc_meta_Publisher,
    vlc_meta_EncodedBy,
    vlc_meta_ArtworkURL,
    vlc_meta_TrackID
} vlc_meta_type_t;

#define VLC_META_TYPE_COUNT 17

#define ITEM_PREPARSED       1
#define ITEM_ARTURL_FETCHED  2
#define ITEM_ART_FETCHED     4
#define ITEM_ART_NOTFOUND    8

/**
 * Basic function to deal with meta
 */
struct vlc_meta_t;

VLC_EXPORT(vlc_meta_t *, vlc_meta_New, ( void ));
VLC_EXPORT(void,         vlc_meta_Delete, ( vlc_meta_t *m ));
VLC_EXPORT(void,         vlc_meta_Set, ( vlc_meta_t *p_meta, vlc_meta_type_t meta_type, const char *psz_val ));
VLC_EXPORT(const char *, vlc_meta_Get, ( const vlc_meta_t *p_meta, vlc_meta_type_t meta_type ));

VLC_EXPORT(void,         vlc_meta_AddExtra, ( vlc_meta_t *m, const char *psz_name, const char *psz_value ));
VLC_EXPORT(const char *, vlc_meta_GetExtra, ( const vlc_meta_t *m, const char *psz_name ));
VLC_EXPORT(unsigned,     vlc_meta_GetExtraCount, ( const vlc_meta_t *m ));

/**
 * Allocate a copy of all extra meta names and a table with it.
 * Be sure to free both the returned pointers and its name.
 */
VLC_EXPORT(char **,      vlc_meta_CopyExtraNames, ( const vlc_meta_t *m ));

VLC_EXPORT(void,         vlc_meta_Merge, ( vlc_meta_t *dst, const vlc_meta_t *src ));

VLC_EXPORT(int,  vlc_meta_GetStatus, ( vlc_meta_t *m ));
VLC_EXPORT(void, vlc_meta_SetStatus, ( vlc_meta_t *m, int status ));

/**
 * Returns a localizes string describing the meta
 */
VLC_EXPORT(const char *, vlc_meta_TypeToLocalizedString, ( vlc_meta_type_t meta_type ) );

enum {
    ALBUM_ART_WHEN_ASKED,
    ALBUM_ART_WHEN_PLAYED,
    ALBUM_ART_ALL
};


typedef struct meta_export_t
{
    VLC_COMMON_MEMBERS
    input_item_t *p_item;
    const char *psz_file;
} meta_export_t;

VLC_EXPORT( int, input_item_WriteMeta, (vlc_object_t *, input_item_t *) );

/* Setters for meta.
 * Warning: Make sure to use the input_item meta setters (defined in vlc_input.h)
 * instead of those one. */
#define vlc_meta_SetTitle( meta, b )       vlc_meta_Set( meta, vlc_meta_Title, b )
#define vlc_meta_SetArtist( meta, b )      vlc_meta_Set( meta, vlc_meta_Artist, b )
#define vlc_meta_SetGenre( meta, b )       vlc_meta_Set( meta, vlc_meta_Genre, b )
#define vlc_meta_SetCopyright( meta, b )   vlc_meta_Set( meta, vlc_meta_Copyright, b )
#define vlc_meta_SetAlbum( meta, b )       vlc_meta_Set( meta, vlc_meta_Album, b )
#define vlc_meta_SetTrackNum( meta, b )    vlc_meta_Set( meta, vlc_meta_TrackNumber, b )
#define vlc_meta_SetDescription( meta, b ) vlc_meta_Set( meta, vlc_meta_Description, b )
#define vlc_meta_SetRating( meta, b )      vlc_meta_Set( meta, vlc_meta_Rating, b )
#define vlc_meta_SetDate( meta, b )        vlc_meta_Set( meta, vlc_meta_Date, b )
#define vlc_meta_SetSetting( meta, b )     vlc_meta_Set( meta, vlc_meta_Setting, b )
#define vlc_meta_SetURL( meta, b )         vlc_meta_Set( meta, vlc_meta_URL, b )
#define vlc_meta_SetLanguage( meta, b )    vlc_meta_Set( meta, vlc_meta_Language, b )
#define vlc_meta_SetNowPlaying( meta, b )  vlc_meta_Set( meta, vlc_meta_NowPlaying, b )
#define vlc_meta_SetPublisher( meta, b )   vlc_meta_Set( meta, vlc_meta_Publisher, b )
#define vlc_meta_SetEncodedBy( meta, b )   vlc_meta_Set( meta, vlc_meta_EncodedBy, b )
#define vlc_meta_SetArtURL( meta, b )      vlc_meta_Set( meta, vlc_meta_ArtworkURL, b )
#define vlc_meta_SetTrackID( meta, b )     vlc_meta_Set( meta, vlc_meta_TrackID, b )

#define VLC_META_TITLE              vlc_meta_TypeToLocalizedString( vlc_meta_Title )
#define VLC_META_ARTIST             vlc_meta_TypeToLocalizedString( vlc_meta_Artist )
#define VLC_META_GENRE              vlc_meta_TypeToLocalizedString( vlc_meta_Genre )
#define VLC_META_COPYRIGHT          vlc_meta_TypeToLocalizedString( vlc_meta_Copyright )
#define VLC_META_ALBUM              vlc_meta_TypeToLocalizedString( vlc_meta_Album )
#define VLC_META_TRACK_NUMBER       vlc_meta_TypeToLocalizedString( vlc_meta_TrackNumber )
#define VLC_META_DESCRIPTION        vlc_meta_TypeToLocalizedString( vlc_meta_Description )
#define VLC_META_RATING             vlc_meta_TypeToLocalizedString( vlc_meta_Rating )
#define VLC_META_DATE               vlc_meta_TypeToLocalizedString( vlc_meta_Date )
#define VLC_META_SETTING            vlc_meta_TypeToLocalizedString( vlc_meta_Setting )
#define VLC_META_URL                vlc_meta_TypeToLocalizedString( vlc_meta_URL )
#define VLC_META_LANGUAGE           vlc_meta_TypeToLocalizedString( vlc_meta_Language )
#define VLC_META_NOW_PLAYING        vlc_meta_TypeToLocalizedString( vlc_meta_NowPlaying )
#define VLC_META_PUBLISHER          vlc_meta_TypeToLocalizedString( vlc_meta_Publisher )
#define VLC_META_ENCODED_BY         vlc_meta_TypeToLocalizedString( vlc_meta_EncodedBy )
#define VLC_META_ART_URL            vlc_meta_TypeToLocalizedString( vlc_meta_ArtworkURL )
#define VLC_META_TRACKID            vlc_meta_TypeToLocalizedString( vlc_meta_TrackID )


#endif
