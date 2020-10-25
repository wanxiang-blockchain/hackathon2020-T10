import React, { useState, useEffect } from 'react'
import SWR from 'swr'
import { Button, Table, Form, Breadcrumb, Input } from 'antd'
import moment from 'moment'
import qs from 'qs'
import { MessageOutlined, LikeOutlined, StarOutlined } from '@ant-design/icons';
import { useQueryList, usePagination, useRowSelection } from '../utils/hooks'
import ModalVerify from '../components/ModalVerify'
import '../styles/Home.module.less'


const Verify = () => {

  const { loading, data, pagination, setCurrentPage, filter, setFilter  } = useQueryList('exam-answers')

  const columns = [
    { title: 'id', dataIndex: 'id', key: 'id' },
    { title: '笔编号', dataIndex: 'GpenID', key: 'GpenID' },
    { title: '考生', dataIndex: 'StuName', key: 'StuName' },
    { title: '签名', key: 'Sign', dataIndex: 'Sign', width: '400px', ellipsis: true },
    { title: 'PoEHash', key: 'PoEHash', dataIndex: 'PoEHash', width: '200px' },
    { title: '上链时间', key: 'CurrTime', dataIndex: 'CurrTime', },
    { title: '操作', key: 'actions', dataIndex: 'actions',
      render : (text, record) => {
        return  (
          <a onClick={()=> setVerifyData({ visible: true, verifyData: record })} >
            <span style={{color: '#ff7875'}}> 链上验真</span> 
          </a>
        )
      } 
    }
  ]

  const [verifyData, setVerifyData] = useState({ visible: false })

  // Table -- selected rows
  const { rowSelection } = useRowSelection()

  // do search
  const onSubmit = values => {
    console.log('onSubmit', values)
    const { userName, penId } = values
    setFilter({ GpenID: penId && penId.length > 0 ? penId : undefined, 
      StuName: userName && userName.length > 0 ? userName : undefined })
  }

  return (
    <main className='main'>
      {
        verifyData && verifyData.visible ? (
          <ModalVerify 
            visible={verifyData.visible}
            verifyData={verifyData.verifyData}
            handleOk={ ()=> setVerifyData({ ...verifyData, visible: false }) }
            handleCancel={ ()=> setVerifyData({ ...verifyData, visible: false }) }
          />
        ) : null

      }
      <h1 className='title'> 验真 </h1>
      <div style={{paddingLeft: '50px', width: '100%'}}>
        <Breadcrumb>
          <Breadcrumb.Item>
            <a href='/'>Home </a>
          </Breadcrumb.Item>
          <Breadcrumb.Item> Verify </Breadcrumb.Item>
        </Breadcrumb>
      </div>

      <div style={{marginTop: '20px', paddingLeft: '80px', width: '100%'}}>
        <Form  name='login' layout="inline"  onFinish={onSubmit} > 
          <Form.Item label='笔编号' name='penId'
            rules = { [{ required: false, message: 'Please input the exam title!' }]}
          >
            <Input placeholder="笔编号" />
          </Form.Item>
          <Form.Item label='考生名' name='userName'
            rules = { [{ required: false, message: 'Please input the candidate name!' }]}
          >
            <Input placeholder="考生名" />
          </Form.Item>
          <Button type="primary" htmlType="submit" className="login-form-button"> 查询 </Button>

        </Form>
      </div>

      <div style={{padding: '20px 50px', width: '100%'}}>
        <Table 
          dataSource={data}
          columns={columns} 
          loading={loading}
          rowSelection={rowSelection}
          pagination={pagination}
          bordered={true}/>
      </div>

    </main>
  )
}


export default Verify
