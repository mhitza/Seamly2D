/************************************************************************
 **
 **  @file   vcontainer.h
 **  @author Roman Telezhynskyi <dismine(at)gmail.com>
 **  @date   November 15, 2013
 **
 **  @brief
 **  @copyright
 **  This source code is part of the Valentine project, a pattern making
 **  program, whose allow create and modeling patterns of clothing.
 **  Copyright (C) 2013-2015 Valentina project
 **  <https://bitbucket.org/dismine/valentina> All Rights Reserved.
 **
 **  Valentina is free software: you can redistribute it and/or modify
 **  it under the terms of the GNU General Public License as published by
 **  the Free Software Foundation, either version 3 of the License, or
 **  (at your option) any later version.
 **
 **  Valentina is distributed in the hope that it will be useful,
 **  but WITHOUT ANY WARRANTY; without even the implied warranty of
 **  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 **  GNU General Public License for more details.
 **
 **  You should have received a copy of the GNU General Public License
 **  along with Valentina.  If not, see <http://www.gnu.org/licenses/>.
 **
 *************************************************************************/

#ifndef VCONTAINER_H
#define VCONTAINER_H

#include <qcompilerdetection.h>
#include <QCoreApplication>
#include <QHash>
#include <QMap>
#include <QMessageLogger>
#include <QSet>
#include <QSharedPointer>
#include <QSharedData>
#include <QSharedDataPointer>
#include <QString>
#include <QStringList>
#include <QTypeInfo>
#include <QtGlobal>
#include <new>

#include "../vmisc/def.h"
#include "../ifc/exception/vexceptionbadid.h"
#include "../ifc/ifcdef.h"
#include "../vgeometry/vabstractcubicbezierpath.h"
#include "../vgeometry/vabstractcurve.h"
#include "../vgeometry/vgobject.h"
#include "../vmisc/diagnostic.h"
#include "variables.h"
#include "variables/vinternalvariable.h"
#include "vdetail.h"
#include "vpiece.h"
#include "vtranslatevars.h"

class VAbstractCubicBezierPath;
class VAbstractCurve;
class VArc;
class VArcRadius;
class VCurveAngle;
class VCurveLength;
class VGObject;
class VIncrement;
class VLengthLine;
class VLineAngle;
class VMeasurement;
class VTranslateVars;

QT_WARNING_PUSH
QT_WARNING_DISABLE_GCC("-Weffc++")
QT_WARNING_DISABLE_INTEL(2021)

class VContainerData : public QSharedData //-V690
{
public:

    VContainerData(const VTranslateVars *trVars, const Unit *patternUnit)
        : gObjects(QHash<quint32, QSharedPointer<VGObject> >()),
          variables(QHash<QString, QSharedPointer<VInternalVariable> > ()),
          details(QSharedPointer<QHash<quint32, VDetail>>(new QHash<quint32, VDetail>())),
          pieces(QSharedPointer<QHash<quint32, VPiece>>(new QHash<quint32, VPiece>())),
          trVars(trVars), patternUnit(patternUnit)
    {}

    VContainerData(const VContainerData &data)
        : QSharedData(data),
          gObjects(data.gObjects),
          variables(data.variables),
          details(data.details),
          trVars(data.trVars),
          patternUnit(data.patternUnit)
    {}

    virtual ~VContainerData();

    /**
     * @brief gObjects graphicals objects of pattern.
     */
    QHash<quint32, QSharedPointer<VGObject> > gObjects;

    /**
     * @brief variables container for measurements, increments, lines lengths, lines angles, arcs lengths, curve lengths
     */
    QHash<QString, QSharedPointer<VInternalVariable>> variables;
    /**
     * @brief details container of details
     */
    QSharedPointer<QHash<quint32, VDetail>> details;
    QSharedPointer<QHash<quint32, VPiece>> pieces;

    const VTranslateVars *trVars;
    const Unit *patternUnit;

private:
    VContainerData &operator=(const VContainerData &) Q_DECL_EQ_DELETE;
};

QT_WARNING_POP

/**
 * @brief The VContainer class container of all variables.
 */
class VContainer
{
    Q_DECLARE_TR_FUNCTIONS(VContainer)
public:
    VContainer(const VTranslateVars *trVars, const Unit *patternUnit);
    VContainer &operator=(const VContainer &data);
    VContainer(const VContainer &data);
    ~VContainer();

    template <typename T>
    const QSharedPointer<T> GeometricObject(const quint32 &id) const;
    const QSharedPointer<VGObject> GetGObject(quint32 id) const;
    static const QSharedPointer<VGObject> GetFakeGObject(quint32 id);
    const VDetail      GetDetail(quint32 id) const;
    VPiece             GetPiece(quint32 id) const;
    qreal              GetTableValue(const QString& name, MeasurementsType patternType) const;
    template <typename T>
    QSharedPointer<T>  GetVariable(QString name) const;
    static quint32     getId();
    static quint32     getNextId();
    static void        UpdateId(quint32 newId);

    quint32            AddGObject(VGObject *obj);
    quint32            AddDetail(const VDetail &detail);
    quint32            AddPiece(const VPiece &detail);
    void               AddLine(const quint32 &firstPointId, const quint32 &secondPointId);
    void               AddArc(const QSharedPointer<VArc> &arc, const quint32 &arcId, const quint32 &parentId = NULL_ID);
    void               AddSpline(const QSharedPointer<VAbstractBezier> &curve, quint32 id, quint32 parentId = NULL_ID);
    void               AddCurveWithSegments(const QSharedPointer<VAbstractCubicBezierPath> &curve, const quint32 &id,
                                            quint32 parentId = NULL_ID);

    template <typename T>
    void               AddVariable(const QString& name, T *var);
    void               RemoveVariable(const QString& name);

    void               UpdateGObject(quint32 id, VGObject* obj);
    void               UpdateDetail(quint32 id, const VDetail &detail);
    void               UpdatePiece(quint32 id, const VPiece &detail);

    void               Clear();
    void               ClearForFullParse();
    void               ClearGObjects();
    void               ClearCalculationGObjects();
    void               ClearVariables(const VarType &type = VarType::Unknown);
    void               ClearDetails();
    static void        ClearUniqueNames();

    static void        SetSize(qreal size);
    static void        SetHeight(qreal height);
    static qreal       size();
    static qreal      *rsize();
    static qreal       height();
    static qreal      *rheight();

    bool               VariableExist(const QString& name);

    void               RemoveIncrement(const QString& name);

    const QHash<quint32, QSharedPointer<VGObject> >         *DataGObjects() const;
    const QHash<quint32, VDetail>                           *DataDetails() const;
    const QHash<quint32, VPiece>                            *DataPieces() const;
    const QHash<QString, QSharedPointer<VInternalVariable>> *DataVariables() const;

    const QMap<QString, QSharedPointer<VMeasurement> >  DataMeasurements() const;
    const QMap<QString, QSharedPointer<VIncrement> >    DataIncrements() const;
    const QMap<QString, QSharedPointer<VLengthLine> >   DataLengthLines() const;
    const QMap<QString, QSharedPointer<VCurveLength> >  DataLengthCurves() const;
    const QMap<QString, QSharedPointer<VCurveCLength> > DataCurvesCLength() const;
    const QMap<QString, QSharedPointer<VLineAngle> >    DataAngleLines() const;
    const QMap<QString, QSharedPointer<VArcRadius> >    DataRadiusesArcs() const;
    const QMap<QString, QSharedPointer<VCurveAngle> >   DataAnglesCurves() const;

    const QHash<QString, qreal *> PlainVariables() const;

    static bool        IsUnique(const QString &name);
    static QStringList AllUniqueNames();

    const Unit *GetPatternUnit() const;
    const VTranslateVars *GetTrVars() const;

private:
    /**
     * @brief _id current id. New object will have value +1. For empty class equal 0.
     */
    static quint32 _id;
    static qreal   _size;
    static qreal   _height;
    static QSet<QString> uniqueNames;

    QSharedDataPointer<VContainerData> d;

    void AddCurve(const QSharedPointer<VAbstractCurve> &curve, const quint32 &id, quint32 parentId = NULL_ID);

    template <class T>
    uint qHash( const QSharedPointer<T> &p );

    template <typename key, typename val>
    // cppcheck-suppress functionStatic
    const val GetObject(const QHash<key, val> &obj, key id) const;

    template <typename val>
    void UpdateObject(QHash<quint32, val > &obj, const quint32 &id, val point);

    template <typename key, typename val>
    static quint32 AddObject(QHash<key, val> &obj, val value);

    template <typename T>
    const QMap<QString, QSharedPointer<T> > DataVar(const VarType &type) const;
};

Q_DECLARE_TYPEINFO(VContainer, Q_MOVABLE_TYPE);

/*
*  Defintion of templated member functions of VContainer
*/

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
const QSharedPointer<T> VContainer::GeometricObject(const quint32 &id) const
{
    QSharedPointer<VGObject> gObj = QSharedPointer<VGObject>();
   if (d->gObjects.contains(id))
   {
       gObj = d->gObjects.value(id);
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), id);
    }
    try
    {
        QSharedPointer<T> obj = qSharedPointerDynamicCast<T>(gObj);
        SCASSERT(obj.isNull() == false);
        return obj;
     }
     catch (const std::bad_alloc &)
     {
        throw VExceptionBadId(tr("Can't cast object"), id);
     }
}


//---------------------------------------------------------------------------------------------------------------------
/**
* @brief GetVariable return varible by name
* @param name variable's name
* @return variable
*/
template <typename T>
QSharedPointer<T> VContainer::GetVariable(QString name) const
{
    SCASSERT(name.isEmpty()==false);
    if (d->variables.contains(name))
    {
        try
        {
            QSharedPointer<T> value = qSharedPointerDynamicCast<T>(d->variables.value(name));
            SCASSERT(value.isNull() == false);
            return value;
        }
        catch (const std::bad_alloc &)
        {
            throw VExceptionBadId(tr("Can't cast object"), name);
        }
    }
    else
    {
        throw VExceptionBadId(tr("Can't find object"), name);
    }
}

//---------------------------------------------------------------------------------------------------------------------
template <typename T>
void VContainer::AddVariable(const QString& name, T *var)
{
    if (d->variables.contains(name))
    {
        if (d->variables.value(name)->GetType() == var->GetType())
        {
            d->variables[name].clear();
        }
        else
        {
            throw VExceptionBadId(tr("Can't find object. Type mismatch."), name);
        }
    }
    d->variables.insert(name, QSharedPointer<T>(var));
    uniqueNames.insert(name);
}

//---------------------------------------------------------------------------------------------------------------------
template <class T>
uint VContainer::qHash( const QSharedPointer<T> &p )
{
    return qHash( p.data() );
}
#endif // VCONTAINER_H
