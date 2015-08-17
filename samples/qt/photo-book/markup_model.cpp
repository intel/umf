// Copyright (C) 2013, Intel Corporation, all rights reserved.

#include "markup_model.hpp"
#include "metadata_helper.hpp"

MarkupModel::MarkupModel()
    : streamPtr(NULL)
{
}

void MarkupModel::loadPeopleInfo(vmf::MetadataStream* stream)
{
    streamPtr = stream;

    loadMarkup();
    loadAssocMarkup();
}

void MarkupModel::loadMarkup()
{
    unassociatedRegions.clear();

    std::vector<vmf::FieldValue> v;
    v.push_back(vmf::FieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) UNASSOCIATED_RECT_TYPE));

    auto markupInfo = streamPtr->queryByNameAndFields( MetadataHelper::MARKUP_DESC_NAME,
        v);
    for (size_t i = 0; i < markupInfo.size(); i++)
    {
        auto item = markupInfo[i];

        vmf::vmf_integer frameIndex;

        try
        {
            frameIndex = item->getFrameIndex();

            FaceRect rct;

            rct.r.x = (vmf::vmf_real) item->getFieldValue(FieldNameConstants::X_RECT_FIELD);
            rct.r.y = (vmf::vmf_real) item->getFieldValue(FieldNameConstants::Y_RECT_FIELD);
            rct.r.width = (vmf::vmf_real) item->getFieldValue(FieldNameConstants::WIDTH_RECT_FIELD);
            rct.r.height = (vmf::vmf_real) item->getFieldValue(FieldNameConstants::HEIGHT_RECT_FIELD);

            unassociatedRegions[frameIndex].push_back(rct);
        }
        catch (vmf::Exception&)
        {
            break;
        }
    }

    v[0] = vmf::FieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) EMPTY_RECT_TYPE);
    markupInfo = streamPtr->queryByNameAndFields( MetadataHelper::MARKUP_DESC_NAME,
        v);
    for (size_t i = 0; i < markupInfo.size(); i++)
    {
        auto item = markupInfo[i];

        vmf::vmf_integer frameIndex;

        try
        {
            frameIndex = item->getFrameIndex();
            unassociatedRegions[frameIndex] = std::vector<FaceRect>();
        }
        catch (vmf::Exception&)
        {
            break;
        }
    }
}

void MarkupModel::storeMarkup()
{
    std::vector<vmf::FieldValue> v;
    v.push_back(vmf::FieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) UNASSOCIATED_RECT_TYPE));

    auto markupInfo = streamPtr->queryByNameAndFields( MetadataHelper::MARKUP_DESC_NAME,
        v);
    streamPtr->remove(markupInfo);

    auto schema = streamPtr->getSchema(MetadataHelper::PHOTO_BOOK_SCHEMA_NAME);
    auto markupDesc = schema->findMetadataDesc(MetadataHelper::MARKUP_DESC_NAME);

    if (markupDesc)
    {
        std::for_each(unassociatedRegions.begin(), unassociatedRegions.end(), 
            [this, &markupDesc] (UnassociatedRList::const_reference ref)
        {
            std::vector<FaceRect> regions = ref.second;

            if (regions.empty())
            {
                std::shared_ptr<vmf::Metadata> regionMetadata = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(markupDesc));
                regionMetadata->setFieldValue(FieldNameConstants::X_RECT_FIELD, (vmf::vmf_real) 0);
                regionMetadata->setFieldValue(FieldNameConstants::Y_RECT_FIELD, (vmf::vmf_real) 0);
                regionMetadata->setFieldValue(FieldNameConstants::WIDTH_RECT_FIELD, (vmf::vmf_real) 0);
                regionMetadata->setFieldValue(FieldNameConstants::HEIGHT_RECT_FIELD, (vmf::vmf_real) 0);
                regionMetadata->setFieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) EMPTY_RECT_TYPE);
                regionMetadata->setFrameIndex(ref.first);

                streamPtr->add(regionMetadata);
            }
            else
            {
                for (size_t j = 0; j < regions.size(); j++)
                {
                    FaceRect rect = regions[j];

                    std::shared_ptr<vmf::Metadata> regionMetadata = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(markupDesc));
                    regionMetadata->setFieldValue(FieldNameConstants::X_RECT_FIELD, (vmf::vmf_real) rect.r.x);
                    regionMetadata->setFieldValue(FieldNameConstants::Y_RECT_FIELD, (vmf::vmf_real) rect.r.y);
                    regionMetadata->setFieldValue(FieldNameConstants::WIDTH_RECT_FIELD, (vmf::vmf_real) rect.r.width);
                    regionMetadata->setFieldValue(FieldNameConstants::HEIGHT_RECT_FIELD, (vmf::vmf_real) rect.r.height);
                    regionMetadata->setFieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) UNASSOCIATED_RECT_TYPE);
                    regionMetadata->setFrameIndex(ref.first);

                    streamPtr->add(regionMetadata);
                }
            }
        });
    }
}

void MarkupModel::setUnassociatedRegions(FrameIndex index,
    const RegionList& list)
{
    unassociatedRegions[index] = list;
}

auto MarkupModel::getUnassociatedRegions(FrameIndex index) -> RegionList
{
    return unassociatedRegions[index];
}

bool MarkupModel::isAlreadyViewed(FrameIndex index)
{
    return unassociatedRegions.find(index) != unassociatedRegions.end();
}

void MarkupModel::deleteUnassociatedRegion(quint64 frameIndex, quint64 rectId)
{
    if (isAlreadyViewed(frameIndex))
    {
        RegionList list = unassociatedRegions.at(frameIndex);

        RegionList::iterator it = std::remove_if(list.begin(), list.end(), [rectId] (const FaceRect& r)
        {
            return r.getIndex() == rectId;
        });

        unassociatedRegions[frameIndex] = RegionList(list.begin(), it);
    }
}

void MarkupModel::addUnassociatedRegion(quint64 frameIndex, const FaceRect& rect)
{
    unassociatedRegions[frameIndex].push_back(rect);
}

void MarkupModel::resizeUnassociatedRegion(quint64 frameIndex, const FaceRect& rect)
{
    if (isAlreadyViewed(frameIndex))
    {
        RegionList list = unassociatedRegions.at(frameIndex);
        RegionList::iterator it = std::find_if(list.begin(), list.end(), [rect] (const FaceRect& r)
        {
            return r.getIndex() == rect.getIndex();
        });

        if (it != list.end())
        {
            it->r = rect.r;
        }

        unassociatedRegions[frameIndex] = list;
    }
}

auto MarkupModel::getAssociatedRegions(FrameIndex index) -> RegionList
{
    RegionList outList;
    AssociatedRegionList arList = associatedRegions[index];
    outList.resize(arList.size());

    for (size_t i = 0; i < arList.size(); i++)
    {
        outList[i] = arList[i];
    }

    return outList;
}

AssociatedRect MarkupModel::addAssociatedRegion(FrameIndex index, vmf::IdType personId,
    const FaceRect& rect)
{
    AssociatedRect ar;

    ar.r = rect.r;
    ar.personId = personId;

    associatedRegions[index].push_back(ar);

    return ar;
}

bool MarkupModel::findPersonByRegion(FrameIndex frameIndex, quint64 regionIndex, 
    vmf::IdType& outId)
{
    AssociatedRegionList rList = associatedRegions[frameIndex];

    AssociatedRegionList::iterator it = std::find_if(rList.begin(), rList.end(),
        [regionIndex] (AssociatedRegionList::const_reference ref)
    {
        return ref.getIndex() == regionIndex;
    });

    if (it != rList.end())
    {
        outId = it->personId;
        return true;
    }

    return false;
}

void MarkupModel::loadAssocMarkup()
{
    associatedRegions.clear();

    std::vector<vmf::FieldValue> v;
    v.push_back(vmf::FieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) ASSOCIATED_RECT_TYPE));

    auto markupInfo = streamPtr->queryByNameAndFields( MetadataHelper::MARKUP_DESC_NAME,
        v);
    auto peopleInfo = streamPtr->queryByName(MetadataHelper::PEOPLE_DESC_NAME);
    auto referencedPeople = peopleInfo.queryByReference(MetadataHelper::MARKUP_DESC_NAME);

    for (size_t i = 0; i < referencedPeople.size(); i++)
    {
        auto refs = referencedPeople[i]->getAllReferences();

        for (size_t j = 0; j < refs.size(); j++)
        {
            auto item = refs[j];

            vmf::vmf_integer frameIndex;

            try
            {
                frameIndex = item.getReferenceMetadata().lock()->getFrameIndex();
                    

                AssociatedRect rct;

                rct.r.x = (vmf::vmf_real) item.getReferenceMetadata().lock()->getFieldValue(FieldNameConstants::X_RECT_FIELD);
                rct.r.y = (vmf::vmf_real) item.getReferenceMetadata().lock()->getFieldValue(FieldNameConstants::Y_RECT_FIELD);
                rct.r.width = (vmf::vmf_real) item.getReferenceMetadata().lock()->getFieldValue(FieldNameConstants::WIDTH_RECT_FIELD);
                rct.r.height = (vmf::vmf_real) item.getReferenceMetadata().lock()->getFieldValue(FieldNameConstants::HEIGHT_RECT_FIELD);
                rct.personId = referencedPeople[i]->getId();

                associatedRegions[frameIndex].push_back(rct);
            }
            catch (vmf::Exception&)
            {
                // Nothing to do
            }
        }
    }
}

void MarkupModel::storeAssociations()
{
    std::vector<vmf::FieldValue> v;
    v.push_back(vmf::FieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) ASSOCIATED_RECT_TYPE));

    auto markupInfo = streamPtr->queryByNameAndFields( MetadataHelper::MARKUP_DESC_NAME,
        v);
    streamPtr->remove(markupInfo);

    auto schema = streamPtr->getSchema(MetadataHelper::PHOTO_BOOK_SCHEMA_NAME);
    auto markupDesc = schema->findMetadataDesc(MetadataHelper::MARKUP_DESC_NAME);

    if (markupDesc)
    {
        std::for_each(associatedRegions.begin(), associatedRegions.end(), 
            [this, &markupDesc] (AssociatedRList::const_reference ref)
        {
            auto regions = ref.second;

            for (size_t j = 0; j < regions.size(); j++)
            {
                AssociatedRect rect = regions[j];

                std::shared_ptr<vmf::Metadata> regionMetadata = std::shared_ptr<vmf::Metadata>(new vmf::Metadata(markupDesc));
                regionMetadata->setFieldValue(FieldNameConstants::X_RECT_FIELD, (vmf::vmf_real) rect.r.x);
                regionMetadata->setFieldValue(FieldNameConstants::Y_RECT_FIELD, (vmf::vmf_real) rect.r.y);
                regionMetadata->setFieldValue(FieldNameConstants::WIDTH_RECT_FIELD, (vmf::vmf_real) rect.r.width);
                regionMetadata->setFieldValue(FieldNameConstants::HEIGHT_RECT_FIELD, (vmf::vmf_real) rect.r.height);
                regionMetadata->setFieldValue(FieldNameConstants::TYPE_RECT_FIELD, (vmf::vmf_integer) ASSOCIATED_RECT_TYPE);
                regionMetadata->setFrameIndex(ref.first);

                streamPtr->add(regionMetadata);
    
                auto person = streamPtr->getById(rect.personId);
                if (person)
                {
                    person->addReference(regionMetadata);
                }
            }
        });
    }
}

void MarkupModel::changeRectAssociation(FrameIndex frameIndex, quint64 rectIndex,
    vmf::IdType newPersonId)
{
    std::for_each(associatedRegions[frameIndex].begin(), associatedRegions[frameIndex].end(),
        [this, rectIndex, newPersonId] (AssociatedRegionList::reference ref)
    {
        if (ref.getIndex() == rectIndex)
        {
            ref.personId = newPersonId;
        }
    });
}

void MarkupModel::removeAssociatedRegions(vmf::IdType personId)
{
    std::for_each(associatedRegions.begin(), associatedRegions.end(),
        [this, personId] (AssociatedRList::reference ref)
    {
        vmf::IdType id = personId;
        std::vector<AssociatedRect>::iterator it = std::remove_if(ref.second.begin(),
            ref.second.end(), [id] (std::vector<AssociatedRect>::reference ref)
        {
            return ref.personId == id;
        });

        ref.second = std::vector<AssociatedRect>(ref.second.begin(), it);
    });
}

void MarkupModel::deleteAssociatedRegion(quint64 frameIndex, quint64 rectId)
{
    if (isAlreadyViewed(frameIndex))
    {
        auto list = associatedRegions.at(frameIndex);

        auto it = std::remove_if(list.begin(), list.end(), [rectId] (const FaceRect& r)
        {
            return r.getIndex() == rectId;
        });

        associatedRegions[frameIndex] = AssociatedRegionList(list.begin(), it);
    }
}

void MarkupModel::resizeAssociatedRegion(quint64 frameIndex, const FaceRect& rect)
{
    if (isAlreadyViewed(frameIndex))
    {
        auto list = associatedRegions.at(frameIndex);
        auto it = std::find_if(list.begin(), list.end(), [rect] (const FaceRect& r)
        {
            return r.getIndex() == rect.getIndex();
        });

        if (it != list.end())
        {
            it->r = rect.r;
        }

        associatedRegions[frameIndex] = list;
    }
}

void MarkupModel::clear()
{
    associatedRegions.clear();
    unassociatedRegions.clear();
}
