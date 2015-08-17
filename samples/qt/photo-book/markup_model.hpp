// Copyright (C) 2013, Intel Corporation, all rights reserved.

#ifndef __MARKUP_MODEL_H__
#define __MARKUP_MODEL_H__

#include <map>
#include <QObject>

#include "vmf/vmf.hpp"
#include "photo_book.hpp"

class AssociatedRect : public FaceRect
{
public:
    AssociatedRect()
        : personId(0)
    {
    }

public:
    vmf::IdType personId;
};

class MarkupModel : public QObject
{
    Q_OBJECT

public:
    typedef std::vector<FaceRect> RegionList;
    typedef std::vector<AssociatedRect> AssociatedRegionList;
    typedef vmf::vmf_integer FrameIndex;
    typedef std::map<FrameIndex, RegionList> UnassociatedRList;
    typedef std::map<FrameIndex, AssociatedRegionList> AssociatedRList;

public slots:
    void deleteUnassociatedRegion(quint64, quint64);
    void addUnassociatedRegion(quint64, const FaceRect&);
    void resizeUnassociatedRegion(quint64, const FaceRect&);
    void deleteAssociatedRegion(quint64, quint64);
    void resizeAssociatedRegion(quint64, const FaceRect&);

public:
    MarkupModel();

    void loadPeopleInfo(vmf::MetadataStream* stream);
    void setUnassociatedRegions(FrameIndex index, const RegionList& list);
    RegionList getUnassociatedRegions(FrameIndex index);
    bool isAlreadyViewed(FrameIndex index);
    RegionList getAssociatedRegions(FrameIndex index);
    void storeMarkup();
    void storeAssociations();
    AssociatedRect addAssociatedRegion(FrameIndex index, vmf::IdType personId,
        const FaceRect& rect);
    bool findPersonByRegion(FrameIndex frameIndex, quint64 regionIndex, 
        vmf::IdType& outId);
    void changeRectAssociation(FrameIndex frameIndex, quint64 rectIndex,
        vmf::IdType newPersonId);
    void removeAssociatedRegions(vmf::IdType personId);
    void clear();

private:
    void loadMarkup();
    void loadAssocMarkup();

private:
    UnassociatedRList unassociatedRegions;
    vmf::MetadataStream *streamPtr;
    AssociatedRList associatedRegions;
};

#endif /* __MARKUP_MODEL_H__ */
