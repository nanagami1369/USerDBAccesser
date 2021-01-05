<template>
  <modal name="update-user-form-modal" id="update-user-form-modal" :height="400">
    <ValidationObserver :immediate="true" v-slot="{ invalid }">
      <form id="update-user-form">
        <h4>アカウントの更新</h4>
        <label for="update-user-input-name">名前(変更する場合は入力)</label>
        <p>現在の名前：{{ OriginalUser.name }}</p>
        <div>
          <input
            type="text"
            name="名前"
            id="update-user-input-name"
            v-model="updateFormUser.name"
          />
        </div>
        <label for="update-user-input-password">パスワード(8文字以上、変更する場合は入力)</label>
        <ValidationProvider :immediate="true" rules="emptyOrMin8" v-slot="{ errors }">
          <input
            type="password"
            name="パスワード"
            id="update-user-input-password"
            v-model="updateFormUser.pass"
          />
          <span class="error-message">{{ errors[0] }}</span>
        </ValidationProvider>
        <label>状態</label>
        <p>現在の状態：{{ OriginalUser.avail | availToString }}</p>
        <div id="avail-radio-button-aria">
          <label for="avail-radio-true">
            <input
              type="radio"
              name="avail"
              value="true"
              id="avail-radio-true"
              v-model="updateFormUser.avail"
            />有効
          </label>
          <label for="avail-radio-false">
            <input
              type="radio"
              name="avail"
              value="false"
              id="avail-radio-false"
              v-model="updateFormUser.avail"
            />無効
          </label>
          <label for="avail-radio-none">
            <input
              type="radio"
              name="avail"
              value="null"
              id="avail-radio-none"
              v-model="updateFormUser.avail"
            />状態を変更しない
          </label>
        </div>
        <label>権限</label>
        <p>現在の権限：{{ OriginalUser.level | levelToString }}</p>
        <div id="level-radio-button-aria">
          <label for="level-radio-admin">
            <input
              type="radio"
              name="level"
              value="ADMIN"
              id="level-radio-admin"
              v-model="updateFormUser.level"
            />管理者
          </label>
          <label for="level-radio-prem">
            <input
              type="radio"
              name="level"
              value="PREM"
              id="level-radio-prem"
              v-model="updateFormUser.level"
            />プレミア
          </label>
          <label for="level-radio-gen">
            <input
              type="radio"
              name="level"
              value="GEN"
              id="level-radio-gen"
              v-model="updateFormUser.level"
            />一般
          </label>
          <label for="level-radio-try">
            <input
              type="radio"
              name="level"
              value="TRY"
              id="level-radio-try"
              v-model="updateFormUser.level"
            />お試し
          </label>
          <label for="level-radio-none">
            <input
              type="radio"
              name="level"
              value="null"
              id="level-radio-none"
              v-model="updateFormUser.level"
            />権限を変更しない
          </label>
        </div>
        <button id="send-form-update-user-button" type="button" @click="submit" :disabled="invalid">
          アカウントを更新する
        </button>
        <p v-show="invalid" class="error-message">入力ミスがあります</p>
      </form>
    </ValidationObserver>
  </modal>
</template>

<script lang="ts">
import { Component, Prop, Vue } from 'vue-property-decorator'
import { ValidationProvider, ValidationObserver, extend } from 'vee-validate'
import { required } from 'vee-validate/dist/rules'
import { User } from '@/model/User'
import { AvailToPrintString, LevelToPrintString } from '@/model/PrintString'

interface UpdateFormUser {
  id: number
  name: string
  pass: string
  avail: 'null' | 'true' | 'false'
  level: 'null' | 'ADMIN' | 'PREM' | 'GEN' | 'TRY'
}

extend('emptyOrMin8', {
  validate: (value: string) => value.length === 0 || value.length >= 8,
  message: '{_field_}は、8文字以上にして下さい'
})
extend('required', {
  ...required,
  message: '{_field_}は必須です'
})
@Component({
  filters: {
    availToString: function(avail: boolean) {
      return AvailToPrintString(avail)
    },
    levelToString: function(level: string) {
      return LevelToPrintString(level)
    }
  },
  components: { ValidationProvider, ValidationObserver }
})
export default class UpdateUserFormModal extends Vue {
  @Prop() public OriginalUser?: User

  public updateFormUser: UpdateFormUser = {
    id: -1,
    name: '',
    pass: '',
    avail: 'null',
    level: 'null'
  }

  public submit() {
    if (this.OriginalUser === undefined) {
      alert('アップデート元のデータがありません')
      return
    }

    // id
    let id = -1
    id = this.OriginalUser.id
    // 名前
    let name = ''
    if (this.updateFormUser.name.length !== 0) {
      name = this.updateFormUser.name
    } else {
      name = this.OriginalUser.name
    }
    // パスワード
    let password = ''
    if (this.updateFormUser.pass.length !== 0) {
      password = this.updateFormUser.pass
    }
    // 状態
    let avail = false
    if (this.updateFormUser.avail !== 'null') {
      avail = this.updateFormUser.avail === 'true' ? true : false
    } else {
      avail = this.OriginalUser.avail
    }
    // 権限
    let level = 'ADMIN'
    if (this.updateFormUser.level !== 'null') {
      level = this.updateFormUser.level
    } else {
      level = this.OriginalUser.level
    }
    const sendUpdatedUser: User = {
      id: id,
      name: name,
      pass: password,
      avail: avail,
      level: level
    }
    this.$emit('submit', sendUpdatedUser)
    this.updateFormUser = {
      id: -1,
      name: '',
      pass: '',
      avail: 'null',
      level: 'null'
    }
    this.$modal.hide('update-user-form-modal')
  }
}
</script>
<style scoped>
#update-user-form {
  display: flex;
  flex-direction: column;
  margin: 10px;
}
#send-form-update-user-button {
  font-size: 1em;
  margin: 0.5rem 5rem;
}

#send-form-update-user-button:disabled {
  background-color: gray;
}

.error-message {
  text-decoration: underline;
}
.error-message:not(:empty)::before {
  content: '※ ';
  color: red;
}
</style>
