#ifndef ENOBIO_H
#define ENOBIO_H

/*!
 * \mainpage
 *
 * The Enobio API consists on a set of classes that permit the access to the
 * Enobio device and the data that it provides.
 *
 * The main class to access the device is Enobio. This class provides methods
 * for opening the device, starting and stopping the data streaming and
 * configuring the device.
 *
 * The device configuration is made through the Property class which shall be
 * set with a valid property name and its value. The valid property names are
 * available as static strings by the Enobio class (they all start with
 * "STR_PROPERTY_" prefix).
 *
 * The access to both the data streaming and the device status is made through
 * a producer-consumer protocol. Enobio implements two producers, one for the
 * data streaming (Enobio::ENOBIO_DATA) and another one for its status
 * (Enobio::STATUS). By registering an implementation of the IDataConsumer
 * virtual class with the Enobio::registerConsumer method the link between the
 * poducer and the cosumer is made. Thus the Enobio class will call the
 * IDataConsumer::receiveData method of all registered IDataConsumer
 * implementations every time a new sample is available or the device status
 * changes.
 *
 * The actual data or status is passed as a parameter to the
 * IDataConsumer::receiveData implementation through the PData class. The
 * pointer returned by the PData::getData method shall be casted to the
 * EnobioData or StatusData class depending on whether the
 * implementation of the IDataConsumer is registered to the Enobio::ENOBIO_DATA
 * producer or the Enobio::STATUS one.
 *
 * The implementation of the IDataConsumer::receiveData method shall take into
 * consideration that it will be executed from a different thread than teh
 * Enobio one. So the operations done within the implementation shall be
 * thread-safe.
 *
 * Another consideration that the IDataConsumer::receiveData implementation
 * shall take into account is that the data accessed through the PData::getData
 * method is not accessible out of the scope of that method. That data is
 * deleted inside the Enobio class when all the IDataConsumer implementations
 * are called.
 */

#include "EnobioAPI_global.h"
#include "IDataProcessor.h"
#include "IDataProducer.h"
#include "Property.h"
#include "IDataProcessor.h"
#include "EnobioData.h"
#include "StatusData.h"


/*! \class Enobio Enobio.h
 *
 * \brief This class provides access to the Enobio device and its data
 * streaming.
 *
 * This class provides methods for opening the device, starting and stopping
 * the data streaming and configuring the device.
 *
 * It inheritates from the base class IDataProcessor so it provides producer
 * implementations to which IDataConsumer implementations can register to
 * access both streaming data and device status.
 */

class ENOBIOAPISHARED_EXPORT Enobio : public IDataProcessor {
public:


    /*!
     * \var Enobio::STR_PROPERTY_USB_FIRMWARE_VERSION
     *
     * This is the string that identifies the USB firmware version property.
     */
    static const char * STR_PROPERTY_USB_FIRMWARE_VERSION;
    /*!
     * \var Enobio::STR_PROPERTY_ENOBIO_FIRMWARE_VERSION
     *
     * This is the string that identifies the Enobio firmware version property.
     */
    static const char * STR_PROPERTY_ENOBIO_FIRMWARE_VERSION;
    /*!
     * \var Enobio::STR_PROPERTY_USB_TRANSMISSION_CHANNEL
     *
     *This is the string that identifies the USB transmission channel property.
     */
    static const char * STR_PROPERTY_USB_TRANSMISSION_CHANNEL;
    /*!
     * \var Enobio::STR_PROPERTY_ENOBIO_TRANSMISSION_CHANNEL
     *
     * This is the string that identifies the Enobio transmission channel
     * property.
     */
    static const char * STR_PROPERTY_ENOBIO_TRANSMISSION_CHANNEL;
    /*!
     * \var Enobio::STR_PROPERTY_ENABLE_CHANNEL_1
     *
     * This is the string that identifies the property to enable or disable the
     * channel 1.
     */
    static const char * STR_PROPERTY_ENABLE_CHANNEL_1;
    /*!
     * \var Enobio::STR_PROPERTY_ENABLE_CHANNEL_2
     *
     * This is the string that identifies the property to enable or disable the
     * channel 2.
     */
    static const char * STR_PROPERTY_ENABLE_CHANNEL_2;
    /*!
     * \var Enobio::STR_PROPERTY_ENABLE_CHANNEL_3
     *
     * This is the string that identifies the property to enable or disable the
     * channel 3.
     */
    static const char * STR_PROPERTY_ENABLE_CHANNEL_3;
    /*!
     * \var Enobio::STR_PROPERTY_ENABLE_CHANNEL_4
     *
     * This is the string that identifies the property to enable or disable the
     * channel 4.
     */
    static const char * STR_PROPERTY_ENABLE_CHANNEL_4;
    /*!
     * \var Enobio::ENOBIO_API_VERSION
     *
     * This is the string that identifies the API version number.
     */
    static const char * ENOBIO_API_VERSION;


    /*!
     * Constructor of an Enobio object
     */
    Enobio ();

    /*!
     * Enobio destructor
     */
    virtual ~Enobio ();


    /*!
     * The IDs of the available Enobio producers.
     */
    enum producerIDs
    {
        /*!
         * Data streaming producer ID
         */
        ENOBIO_DATA=1,
        /*!
         * Device status producer ID
         */
        STATUS=2
    };           

    /*!
     * It performs the required operations to open and initialize the hardware
     * device
     *
     * \return It returns true if the device is opened correctly, false
     * otherwise. When the device is not present or can not be opened an error
     * status is also passed to the IDataConsumer implementations already
     * registered to the Enobio::STATUS producer.
     */
    bool openDevice ();

    /*!
     * It performs the required operations to close the hardware device.
     */
    void closeDevice ();

    /*!
     * It starts the Enobio data sampling. The data streaming will then be
     * received at the IDataConsumer implementations already registered to the
     * Enobio::ENOBIO_DATA producer. If the Enobio device is not turned on or
     * it is out of battery a time-out will occur and the corresponding error
     * status is generated by the Enobio::STATUS producer.
     */
    void startStreaming ();

    /*!
     * It stops the Enobio data sampling. After this call some samples might
     * still be provided by the Enobio::ENOBIO_DATA producer due to the latency
     * in the wireless link.
     */
    void stopStreaming ();

    /*!
     * Checks whether the Enobio device is switched on.
     *
     * \return It returns true if the wireless link between the Enobio device
     * and the USB antenna works fine, false otherwise.
     */
    bool ping ();

    /*!
     * It returns the Property object identified with a property name.
     *
     * \param propertyName Name of the property to be obtained.
     *
     * \return Pointer to the solicited property.
     */
    const Property* getProperty (const char * propertyName);

    /*!
     * It configures an API property to be identical to the parameter.
     *
     * \param property Configuration property object.
     *
     * \return It returns true if the method succeeded on setting the property,
     * false otherwise.
     */
    bool setProperty(const Property& property);


    /*!
     * It returns the IDataConsumer implementation that is identified with the
     * passed id.
     *
     * \param id The identification of the IDataConsumer to be obtained.
     *
     * \return Pointer to the solicited IDataConsumer. If the id does not match
     * with none of the consumers Enobio has the function returns NULL. Since
     * the Enobio class does not have any consumer, this fucntion will always
     * return NULL.
     */
    virtual IDataConsumer * getDataConsumer (const int id);

    /*!
     * It returns the IDataProducer object identified with the passed id.
     *
     * \param id The identification of the DataProducer to be obtained.
     *
     * \return Pointer to the solicited DataProducer. If the id does not match
     * with none of the producers Enobio has the function returns NULL.
     */
    virtual IDataProducer * getDataProducer (const int id);


    /*!
     * It registers an IDataConsumer implementation to the producer identified
     * by the parameter id.
     *
     * \param id Identification of the producer to which the IDataConsumer
     * implementation is registered.
     *
     * \param dataConsumer The DataConsumer to add to the producer.
     *
     * \return If the producer specified does not exist the function returns
     * false.
     *
     */
    virtual bool registerConsumer (const int id,
                                    IDataConsumer& dataConsumer);

    /*!
     * It removes an IDataConsumer implementation from the list of consumers
     * already registered to the producer identified by the id parameter
     *
     * \param id Identification of the producer from which the IDataConsumer
     * implemenation is deregistered
     *
     * \param dataConsumer The DataConsumer to remove from the producer
     *
     * \return If the producer specified does not exist the function returns
     * false
     *
     */
    virtual bool deregisterConsumer (const int id,
                                     const IDataConsumer& dataConsumer);


private:

    /*
     * Instance of the implementation class.
     */
    void * _enobioPrivate;


};

#endif // ENOBIO_H
