import React, {useEffect, useState} from 'react'
import { Modal, Spin, Input, Alert, Divider, Descriptions, DatePicker, message, List, Button } from 'antd';
import moment from 'moment'
import Promise from 'bluebird'
import useSWR from 'swr'
import md5 from 'blueimp-md5'//'../utils/md5'
import { CloseCircleTwoTone, SmileTwoTone, SyncOutlined, HeartTwoTone, CheckCircleTwoTone } from '@ant-design/icons'
import LoadingMask from '../components/LoadingMask'

const DescriptionPanel = ({record, result }) => {

  const { GpenID, StuName, StuIDCardNo, Sign, PoEHash, updated_at, Topic, CurrTime, Answer } = record
  console.log('record', record)

  const source = {
    GpenID,
    StuName,
    StuIDCardNo,
    CurrTime,
    Topic,
    Answer,
    Sign
  }
  const md5Hash = md5( JSON.stringify(source) )
  console.log( 'source ', source )
  console.log(JSON.stringify(source))
  console.log('md5 hash : ', md5Hash)

  const { poeStatus, poeHash, blockNumber, hashContent, timestamp  } = result || {}

  let verified = null, verifyTitle = ''
  if ( md5Hash && hashContent ) {
    verified =  md5Hash == hashContent
    verifyTitle = verified ? '真' : '假'
  }

  return (
    <>
    <Descriptions title='链上存证:' bordered>
      <Descriptions.Item label='存链Hash' span={2}><span className='_bold'>{hashContent}</span></Descriptions.Item>
      <Descriptions.Item label='上链时间'>{timestamp ? new Date(timestamp).toLocaleString() : ''}</Descriptions.Item>
      <Descriptions.Item label='区块' >{blockNumber}</Descriptions.Item>
    </Descriptions>
    <Divider />

    <Descriptions title='待验数据Hash: ' bordered>
      <Descriptions.Item label='Hash'><span className='_bold'>{md5Hash}</span></Descriptions.Item>
        <Descriptions.Item style={{fontSize: '24px', color: '#eb2f96'}} label={verifyTitle} > {
          verified === null ? (
            <SyncOutlined style={{color: 'black'}} spin />
          ) : (
            verified ? <CheckCircleTwoTone twoToneColor="#eb2f96" /> : <CloseCircleTwoTone />
          )
        }</Descriptions.Item>
    </Descriptions>

    <Divider />
    <Descriptions title='待验数据:' bordered size='small' span={4}>
      <Descriptions.Item label='姓名' span={2}>{StuName}</Descriptions.Item>
      <Descriptions.Item label='上链时间' span={2}>{moment(updated_at).format('YYYY-MM-DD HH:mm:ss')}</Descriptions.Item>
      <Descriptions.Item label='笔编号'>{GpenID}</Descriptions.Item>
      <Descriptions.Item label='题目'>{Topic}</Descriptions.Item>
      <Descriptions.Item label='答题'>{ Answer }</Descriptions.Item>
      <Descriptions.Item label='签名' span={3} ><span style={{width: '240px',textOverflow: 'ellipsis'}}>{Sign}</span></Descriptions.Item>
    </Descriptions>
    <Divider />
    <style jsx>{`

    ._bold {
      color: #612500;
      background: #ffec3d;
    }
    `}</style>
    </>
  )
}

const ModalVerify = ({
  visible,
  handleOk,
  handleCancel,
  verifyData
}) => {

  const { PoEHash } = verifyData || {}

  const [loading, setLoading] = useState(true)

  const { data, error } = useSWR([PoEHash], () => {
    if (!PoEHash) {
      return null
    }

    const url = `/api/PoEHash?PoEHash=${PoEHash}`
    return fetch( url, {
      method: 'GET',
      mode: 'cors',
      headers: { 'Content-Type': 'application/json' },
    } )
      .then( res => res.json() )
      .then( data => {
        setLoading(false)
        return data?.data
      }) 
      .catch( err => {
        setLoading(false)
        return {
          result: false,
          message: err
        }
      } )
  })


  console.log('query verify ', data, error, PoEHash)

  return (
    <>
    <Modal 
      title={`链上Hash验真`}
      width={840}
      visible={visible}
      destroyOnClose={true}
      onOk={ handleOk }
      onCancel={handleCancel} >
      {
        !loading ? (
          null
        ) : (
          <LoadingMask />
        )
      }
      <DescriptionPanel record={verifyData} result ={data} />

    </Modal>
    </>
  )
}

export default ModalVerify;
