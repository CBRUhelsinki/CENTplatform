#ifndef ENOBIODATA_H
#define ENOBIODATA_H

#include "EnobioAPI_global.h"

/*! \class EnobioData EnobioData.h
 *
 * \brief This class encapsulates an Enobio data sample.
 *
 * An Enobio data sample consists on 4 data channels, two flags with some meta
 * data information and a timestamp
 */

class ENOBIOAPISHARED_EXPORT EnobioData {
public:

    /*!
     * Enumeration with the possible values for the flag parameter
     */
    enum flagValue
    {
        /*!
         * Normal data
         */
        FLAG_NORMAL_DATA = 0,
        /*!
         * This sample has been interpolated since the packet carrying the
         * sample was lost.
         */
        FLAG_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 1 incorporates a positive
         * offset change.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH1,
        /*!
         * The voltage of the sample in channel 2 incorporates a positive
         * offset change.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH2,
        /*!
         * The voltage of the sample in channel 3 incorporates a positive
         * offset change.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH3,
        /*!
         * The voltage of the sample in channel 4 incorporates a positive
         * offset change.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH4,
        /*!
         * The voltage of the sample in channel 1 incorporates a negative
         * offset change.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH1,
        /*!
         * The voltage of the sample in channel 2 incorporates a negative
         * offset change.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH2,
        /*!
         * The voltage of the sample in channel 3 incorporates a negative
         * offset change.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH3,
        /*!
         * The voltage of the sample in channel 4 incorporates a negative
         * offset change.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH4,
        /*!
         * The voltage of the sample in channel 1 incorporates a positive
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH1_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 2 incorporates a positive
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH2_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 3 incorporates a positive
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH3_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 4 incorporates a positive
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_POSITIVE_OFFSET_CHANGE_CH4_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 1 incorporates a negative
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH1_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 2 incorporates a negative
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH2_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 3 incorporates a negative
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH3_ON_LOST_PACKET,
        /*!
         * The voltage of the sample in channel 4 incorporates a negative
         * offset change that ocurred in a earlier sample that was lost.
         */
        FLAG_NEGATIVE_OFFSET_CHANGE_CH4_ON_LOST_PACKET,
    };

    /*!
     * \var EnobioData::NUMBER_OF_CHANNELS
     *
     * The Number of data channels that Enobio provides.
     */
    static const int NUMBER_OF_CHANNELS = 4;

    /*!
     * Constructor of an EnobioData object
     *
     * \param dataCh1 Channel 1 data value.
     *
     * \param dataCh2 Channel 2 data value.
     *
     * \param dataCh3 Channel 3 data value.
     *
     * \param dataCh4 Channel 4 data value.
     *
     * \param flag Flag that informs about the consideration that has to be
     * taken regarding the current Enobio sample.
     *
     * \param offsetChangeIndex It indicates how many samples earlier the
     * offset change happened.
     *
     * \param timestamp This is the timestamp for the sample. The value
     * corresponds to the number of microseconds since 1970-01-01T00:00:00
     * Universal Coordinated Time.
     */
    EnobioData (int dataCh1, int dataCh2, int dataCh3, int dataCh4,
                flagValue flag, unsigned char offsetChangeIndex,
                long long timestamp);

    /*!
     * It Gets the value of channel 1.
     *
     * \return Channel 1 value.
     */
    int getChannel1 () const;

    /*!
     * It sets the value for channel 1.
     *
     * \param data Channel 1 value.
     */
    void setChannel1 (double data);

    /*!
     * It Gets the value of channel 2.
     *
     * \return Channel 2 value.
     */
    int getChannel2 () const;

    /*!
     * It sets the value for channel 2.
     *
     * \param data Channel 2 value.
     */
    void setChannel2 (double data);

    /*!
     * It Gets the value of channel 3.
     *
     * \return Channel 3 value.
     */
    int getChannel3 () const;

    /*!
     * It sets the value for channel 3.
     *
     * \param data Channel 3 value.
     */
    void setChannel3 (double data);

    /*!
     * It Gets the value of channel 4.
     *
     * \return Channel 4 value.
     */
    int getChannel4 () const;

    /*!
     * It sets the value for channel 4.
     *
     * \param data Channel 4 value.
     */
    void setChannel4 (double data);

    /*!
     * It gets the value of a specific channel.
     *
     * \param channel Number of the requested channel. It shall be a number
     * from 1 to EnobioData::NUMBER_OF_CHANNELS.
     *
     * \return Value of the data of the requested channel.
     */
    int getDataFromChannel (unsigned char channel) const;

    /*!
     * It sets the value of a specific channel.
     *
     * \param data Data value to be set to the specific channel.
     *
     * \param channel Number of the channel whose value is set.
     */
    void setDataToChannel (double data, unsigned char channel);

    /*!
     * It gets the flag of the current Enobio data sample.
     *
     * \return It returns a value from the flagValue enumeration.
     */
    flagValue getFlag () const;

    /*!
     * It gets the offset change index.
     *
     * \return It returns how many samples before the current one an offset
     * change happened.
     */
    unsigned char getOffsetChangeIndex () const;

    /*!
     * It gets the timestamp of the current Enobio sample.
     *
     * \return It returns the the number of microseconds since
     * 1970-01-01T00:00:00 Universal Coordinated Time.
     */
    long long getTimestamp () const;

private:

    /*!
     * The value of channel 1
     */
    int _channel1;

    /*!
     * The value of channel 2
     */
    int _channel2;

    /*!
     * The value of channel 3
     */
    int _channel3;

    /*!
     * The value of channel 4
     */
    int _channel4;

    /*!
     * The flag
     */
    flagValue _flag;

    /*!
     * The offset change index
     */
    unsigned char _offsetChangeIndex;

    /*!
     * The timestamp
     */
    long long _timestamp;
};

#endif // ENOBIODATA_H

